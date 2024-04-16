#include "isam_nonclustered_dir.h"

#include "relational_model/system.h"
#include "storage/isam/isam_nonclustered_leaf.h"

IsamNonClusteredDir::IsamNonClusteredDir(
    const IsamNonClustered& isam,
    uint64_t page_number
) :
    isam (isam),
    page (buffer_mgr.get_page(isam.dir_file_id, page_number))
{
    N  = reinterpret_cast<uint64_t*>(page.data());
    keys = reinterpret_cast<int64_t*>(page.data() + sizeof(*N));
    children = reinterpret_cast<int32_t*>(page.data() + sizeof(*N) + max_keys*sizeof(*keys));
}


IsamNonClusteredDir::~IsamNonClusteredDir() {
    page.unpin();
}

int_fast32_t IsamNonClusteredDir::search_leaf(int64_t encoded_key) {
    auto dir_index = search_child_idx(encoded_key);
    auto page_num = children[dir_index];

    if (page_num < 0) { // negative number: pointer to dir
        IsamNonClusteredDir child(isam, -1 * page_num);
        return child.search_leaf(encoded_key);
    } else { // positive number: pointer to leaf
        return page_num;
    }
}


int_fast32_t IsamNonClusteredDir::search_child_idx(int64_t key) {
    int_fast32_t from = 0;
    int_fast32_t to = *N;

    while (from != to) {
        int_fast32_t middle_dir = (from + to + 1) / 2;
        int_fast32_t middle_record = middle_dir-1;

        if (key < keys[middle_record]) {
            to = middle_record;
        } else { // key >= keys[middle_record]
            from = middle_record+1;
        }
    }
    return from;
}


void IsamNonClusteredDir::insert_record(RID rid, int64_t key) {
    auto dir_index = search_child_idx(key);
    auto page_pointer = children[dir_index];

    if (page_pointer < 0) { // negative number: pointer to dir
        IsamNonClusteredDir child(isam, -1 * page_pointer);
        child.insert_record(rid, key);
    } else { // positive number: pointer to leaf
        IsamNonClusteredLeaf child(isam, page_pointer);
        child.insert_record(rid, key);
    }
}


void IsamNonClusteredDir::delete_record(RID rid, int64_t key) {
    auto dir_index = search_child_idx(key);
    auto page_pointer = children[dir_index];

    if (page_pointer < 0) { // negative number: pointer to dir
        IsamNonClusteredDir child(isam, -1 * page_pointer);
        child.delete_record(rid, key);
    } else { // positive number: pointer to leaf
        IsamNonClusteredLeaf child(isam, page_pointer);
        child.delete_record(rid);
    }
}
