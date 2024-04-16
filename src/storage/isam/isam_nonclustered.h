#pragma once

#include <memory>

#include "relational_model/index.h"
#include "relational_model/relation_iter.h"
#include "storage/heap_file/heap_file.h"

class IsamNonClusteredDir;

class IsamNonClustered : public Index {
public:
    IsamNonClustered(const HeapFile& heap_file, int key_col_idx, const std::string& idx_name);

    void insert_record(RID rid) override;

    void delete_record(RID rid) override;

    std::unique_ptr<RelationIter> get_iter(const Value& min, const Value& max) override;

    IndexType get_type() override { return IndexType::NC_ISAM; }

    const HeapFile& heap_file;

    const int key_column_idx;

    const FileId dir_file_id;

    const FileId leaf_file_id;

    // we reuse this attribute in inserts/deletes to reduce allocations
    Record record_buf;

private:
    std::unique_ptr<IsamNonClusteredDir> root;
};
