#pragma once

#include <memory>

#include "relational_model/relation_iter.h"
#include "relational_model/value.h"
#include "storage/heap_file/rid.h"

enum class IndexType {
    NONE,
    NC_ISAM
};

class Index {
friend class Catalog;
public:
    virtual ~Index() = default;

    virtual std::unique_ptr<RelationIter> get_iter(const Value& min, const Value& max) = 0;

    virtual IndexType get_type() = 0;

private:
    virtual void insert_record(RID rid) = 0;

    virtual void delete_record(RID rid) = 0;
};
