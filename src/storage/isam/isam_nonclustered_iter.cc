#include "isam_nonclustered_iter.h"

#include "storage/isam/isam_nonclustered.h"
#include "storage/isam/isam_nonclustered_leaf.h"

IsamNonClusteredIter::IsamNonClusteredIter(
    const IsamNonClustered& isam,
    const Value& min,
    const Value& max,
    uint_fast32_t key_pos,
    uint_fast32_t start_page_number,
    uint_fast32_t end_page_number
) :
    isam (isam),
    min (min),
    max (max),
    key_pos (key_pos),
    start_page_number (start_page_number),
    end_page_number (end_page_number) { }


IsamNonClusteredIter::IsamNonClusteredIter(
    const IsamNonClustered& isam,
    Value&& min,
    Value&& max,
    uint_fast32_t key_pos,
    uint_fast32_t start_page_number,
    uint_fast32_t end_page_number
) :
    isam (isam),
    min (std::move(min)),
    max (std::move(max)),
    key_pos (key_pos),
    start_page_number (start_page_number),
    end_page_number (end_page_number) { }


void IsamNonClusteredIter::begin(Record& _out) {
    out = &_out;
    current_leaf = std::make_unique<IsamNonClusteredLeaf>(isam, start_page_number);

    current_leaf_pos = 0;
    in_overflow = false;
}


void IsamNonClusteredIter::reset() {
    current_leaf = std::make_unique<IsamNonClusteredLeaf>(isam, start_page_number);

    current_leaf_pos = 0;
    in_overflow = false;
}


bool IsamNonClusteredIter::next() {
    while (true) {
        if (current_leaf_pos < *current_leaf->N) {
            auto pos = current_leaf_pos++;
            isam.heap_file.get_record(current_leaf->records[pos].rid, *out);

            auto value = out->values[key_pos];
            if (value >= min && value <= max) {
                return true;
            }
        } else if (*current_leaf->overflow != -1) {
            if (!in_overflow) {
                saved_next_leaf = *current_leaf->next;
                is_overflow_end = current_leaf->page.get_page_number() == end_page_number;
            }
            current_leaf_pos = 0;
            in_overflow = true;
            current_leaf = std::make_unique<IsamNonClusteredLeaf>(isam, *current_leaf->overflow);
        } else if (in_overflow) {
            if (is_overflow_end) {
                return false;
            } else {
                current_leaf_pos = 0;
                in_overflow = false;
                current_leaf = std::make_unique<IsamNonClusteredLeaf>(isam, saved_next_leaf);
            }
        } else { // not in overflow going to next
            if (current_leaf->page.get_page_number() == end_page_number) {
                return false;
            }
            current_leaf_pos = 0;
            in_overflow = false;
            current_leaf = std::make_unique<IsamNonClusteredLeaf>(isam, *current_leaf->next);
        }
    }
    return false;
}
