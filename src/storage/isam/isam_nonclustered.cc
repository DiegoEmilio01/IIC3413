#include "isam_nonclustered.h"

#include <algorithm>
#include <cassert>
#include <random>
#include <set>

#include "relational_model/system.h"
#include "storage/heap_file/heap_file.h"
#include "storage/heap_file/heap_file_page.h"
#include "storage/heap_file/heap_file_iter.h"
#include "storage/isam/isam_nonclustered_dir.h"
#include "storage/isam/isam_nonclustered_iter.h"
#include "storage/isam/isam_nonclustered_leaf.h"


// construct 8 byte integer with first 8 characters
int64_t serialize_string_key(const char* str) {
    uint64_t res = 0;

    int shift_size = 8*7;
    for (int i = 0; i < 8 && *str != '\0'; i++, str++, shift_size -= 8) {
        uint64_t byte64 = static_cast<uint64_t>(*str);
        res |= byte64 << shift_size;
    }

    return res;
}


IsamNonClustered::IsamNonClustered(
    const HeapFile& heap_file,
    int key_column_idx,
    const std::string& idx_name
) :
    heap_file      (heap_file),
    key_column_idx (key_column_idx),
    dir_file_id    (file_mgr.get_file_id(idx_name + ".dir")),
    leaf_file_id   (file_mgr.get_file_id(idx_name + ".leaf")),
    record_buf     (heap_file.schema.datatypes)
{
    root = std::make_unique<IsamNonClusteredDir>(*this, 0);
    if (*root->N != 0) {
        // Isam was already created
        return;
    }

    std::vector<int64_t> sampled_keys;

    auto iter = heap_file.get_record_iter();

    auto datatypes = heap_file.schema.datatypes;
    auto key_datatype = heap_file.schema.datatypes[key_column_idx];

    Record record(std::move(datatypes));
    iter->begin(record);

    while (iter->next()) {
        auto key = record.values[key_column_idx];
        switch (key_datatype) {
        case DataType::STR: {
            sampled_keys.push_back(serialize_string_key(key.value.as_str));
            break;
        }

        case DataType::INT: {
            sampled_keys.push_back(key.value.as_int);
            break;
        }
        }
    }

    int64_t total_records = sampled_keys.size();

    // Heuristic: try to create directory assuming leafs will be filled at half
    int64_t min_desired_leafs = (1 + ((total_records*2) / IsamNonClusteredLeaf::max_keys));

    int64_t dir_height = 1;
    int64_t total_leafs_pointers = IsamNonClusteredDir::max_keys+1;
    int64_t desired_total_keys   = IsamNonClusteredDir::max_keys;

    while (total_leafs_pointers < min_desired_leafs) {
        dir_height++;
        total_leafs_pointers *= IsamNonClusteredDir::max_keys+1;
        desired_total_keys   *= IsamNonClusteredDir::max_keys+1;
    }

    std::set<uint64_t> key_set; // use set to delete duplicates

    if (total_records <= desired_total_keys) {
        for (auto& key : sampled_keys) {
            key_set.insert(key);
        }
    } else { // desired_total_keys > total_records
        int64_t skip = total_records / desired_total_keys
                     + (total_records % desired_total_keys != 0);
        for (uint64_t i = 0; i < sampled_keys.size(); i+= skip) {
            key_set.insert(sampled_keys[i]);
        }
    }
    assert(key_set.size() <= static_cast<uint64_t>(desired_total_keys));

    // fill with random values
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int64_t> distribution(INT64_MIN, INT64_MAX);

    while (key_set.size() < static_cast<uint64_t>(desired_total_keys)) {
        key_set.insert(distribution(gen));
    }

    std::vector final_keys(key_set.begin(), key_set.end());

    // create directories
    int64_t dirs_in_current_height = 1;
    int64_t current_dir_page_number = 0;

    int current_dir_child = 1;

    // I'm too dumb to figure out how to build the tree in one pass, so I'll do it in two.
    // first to set the children, then to set the keys
    for (int64_t current_height = 0; current_height < dir_height-1; current_height++) {

        // child is dir
        for (int64_t current_dir = 0; current_dir < dirs_in_current_height; current_dir++) {
            IsamNonClusteredDir dir(*this, current_dir_page_number++);

            *dir.N = IsamNonClusteredDir::max_keys;
            for (uint64_t i = 0; i <= IsamNonClusteredDir::max_keys; i++) {
                dir.children[i] = -1 * (current_dir_child++);
            }
            dir.page.make_dirty();
        }
        dirs_in_current_height *= IsamNonClusteredDir::max_keys+1;
    }

    auto saved_dir_page = current_dir_page_number;

    {   // case current_height == dir_height-1
        int current_key = 0;
        int current_leaf_child = 0;
        for (int64_t current_dir = 0; current_dir < dirs_in_current_height; current_dir++) {
            IsamNonClusteredDir dir(*this, current_dir_page_number++);

            *dir.N = IsamNonClusteredDir::max_keys;
            for (uint64_t i = 0; i < IsamNonClusteredDir::max_keys; i++) {
                dir.children[i] = current_leaf_child++;
                dir.keys[i] = final_keys[current_key++];
            }
            dir.children[IsamNonClusteredDir::max_keys] = current_leaf_child++;

            dir.page.make_dirty();
        }
        assert(static_cast<uint64_t>(current_key) == final_keys.size());
    }

    // second pass, to set keys, iterate in reverse
    current_dir_page_number = saved_dir_page;
    for (int64_t current_height = dir_height-1; current_height >= 0; current_height--) {
        dirs_in_current_height /= IsamNonClusteredDir::max_keys+1;

        for (int64_t current_dir = 0; current_dir < dirs_in_current_height; current_dir++) {
            IsamNonClusteredDir dir(*this, --current_dir_page_number);
            for (uint64_t i = 0; i < IsamNonClusteredDir::max_keys; i++) {
                IsamNonClusteredDir child(*this, -1*dir.children[i+1]);
                dir.keys[i] = child.keys[0];
            }
            dir.page.make_dirty();
        }
    }

    // init leafs and connect them
    IsamNonClusteredLeaf first_leaf(*this, 0);
    *first_leaf.N = 0;
    *first_leaf.prev = -1;
    *first_leaf.next = 1;
    *first_leaf.overflow = -1;
    first_leaf.page.make_dirty();

    for (int64_t i = 1; i < total_leafs_pointers - 1; i++) {
        IsamNonClusteredLeaf leaf(*this, i);
        *leaf.N = 0;
        *leaf.prev = i-1;
        *leaf.next = i+1;
        *leaf.overflow = -1;
        leaf.page.make_dirty();
    }

    IsamNonClusteredLeaf last_leaf(*this, total_leafs_pointers - 1);
    *last_leaf.N = 0;
    *last_leaf.prev = total_leafs_pointers - 2;
    *last_leaf.next = -1;
    *last_leaf.overflow = -1;
    last_leaf.page.make_dirty();

    // iterate over all records again and insert
    iter->reset();
    while (iter->next()) {
        auto current_rid = iter->get_current_RID();
        insert_record(current_rid);
    }
}


std::unique_ptr<RelationIter> IsamNonClustered::get_iter(const Value& min, const Value& max) {
    int64_t encoded_min = 0; // assigned to 0 just to avoid compilation warning
    int64_t encoded_max = 0; // assigned to 0 just to avoid compilation warning

    auto key_datatype = heap_file.schema.datatypes[key_column_idx];
    switch (key_datatype) {
    case DataType::STR: {
        encoded_min = serialize_string_key(min.value.as_str);
        encoded_max = serialize_string_key(max.value.as_str);
        break;
    }

    case DataType::INT: {
        encoded_min = min.value.as_int;
        encoded_max = max.value.as_int;
        break;
    }
    }
    auto start_page_num = root->search_leaf(encoded_min);
    auto end_page_num = root->search_leaf(encoded_max);

    return std::make_unique<IsamNonClusteredIter>(
        *this, min, max, key_column_idx, start_page_num, end_page_num
    );
}


void IsamNonClustered::insert_record(RID rid) {
    heap_file.get_record(rid, record_buf);
    auto key = record_buf.values[key_column_idx];

    auto key_datatype = heap_file.schema.datatypes[key_column_idx];
    switch (key_datatype) {
    case DataType::STR: {
        root->insert_record(rid, serialize_string_key(key.value.as_str));
        break;
    }

    case DataType::INT: {
        root->insert_record(rid, key.value.as_int);
        break;
    }
    }
}


void IsamNonClustered::delete_record(RID rid) {
    heap_file.get_record(rid, record_buf);
    auto key = record_buf.values[key_column_idx];

    auto key_datatype = heap_file.schema.datatypes[key_column_idx];
    switch (key_datatype) {
    case DataType::STR: {
        root->delete_record(rid, serialize_string_key(key.value.as_str));
        break;
    }

    case DataType::INT: {
        root->delete_record(rid, key.value.as_int);
        break;
    }
    }
}
