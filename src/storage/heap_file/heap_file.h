#pragma once

#include <memory>
#include <string>
#include <vector>

#include "relational_model/record.h"
#include "storage/file_manager.h"

class RelationIter;
class Schema;

struct RID {
    uint64_t page_num;
    uint64_t dir_slot;

    RID() {}

    RID(uint64_t page_num, uint64_t dir_slot) :
        page_num(page_num), dir_slot(dir_slot) {}
};

class HeapFile {
public:
    const Schema& schema;

    const FileId file_id;

    HeapFile(const Schema& schema, const std::string& table_name);

    // prevent accidental copies
    HeapFile(const HeapFile& other) = delete;

    RID insert_record(Record& record);

    void delete_record(RID rid);

    void vacuum();

    // Iterates over all results
    std::unique_ptr<RelationIter> get_record_iter();
};
