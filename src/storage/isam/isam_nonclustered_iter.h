#pragma once

#include <memory>

#include "relational_model/relation_iter.h"
#include "storage/heap_file/rid.h"

class IsamNonClustered;
class IsamNonClusteredLeaf;

class IsamNonClusteredIter : public RelationIter {
public:
    IsamNonClusteredIter(
        const IsamNonClustered&,
        const Value& min,
        const Value& max,
        uint_fast32_t key_pos,
        uint_fast32_t start_page_number,
        uint_fast32_t end_page_number
    );

    IsamNonClusteredIter(
        const IsamNonClustered&,
        Value&& min,
        Value&& max,
        uint_fast32_t key_pos,
        uint_fast32_t start_page_number,
        uint_fast32_t end_page_number
    );

    virtual void begin(Record& out) override;

    virtual bool next() override;

    virtual void reset() override;

private:
    const IsamNonClustered& isam;

    const Value min;

    const Value max;

    const uint_fast32_t key_pos;

    const uint_fast32_t start_page_number;

    const uint_fast32_t end_page_number;

    std::unique_ptr<IsamNonClusteredLeaf> current_leaf;

    Record* out;

    uint_fast32_t current_leaf_pos;

    bool in_overflow;

    bool is_overflow_end;

    uint_fast32_t saved_next_leaf;
};
