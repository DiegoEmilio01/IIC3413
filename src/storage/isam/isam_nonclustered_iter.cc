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

    // TODO: you may add things here
}


void IsamNonClusteredIter::reset() {
    current_leaf = std::make_unique<IsamNonClusteredLeaf>(isam, start_page_number);

    // TODO: you may add things here
}


bool IsamNonClusteredIter::next() {
    // TODO: implement
    return false;
}
