#pragma once

#include <cstdint>

#include "storage/heap_file/heap_file.h"

class Page;
class Record;

class HeapFilePage {
public:
    Page& page;

    HeapFilePage(Page& page);

    ~HeapFilePage();

    // returns true if record was inserted, false if no space available
    // when the function returns true, the out_record_id is setted
    bool try_insert_record(Record& record, RID* out_record_id);

    void vacuum();

    void get_record(uint32_t dir_pos, Record& out) const;

    void delete_record(uint32_t dir_pos) const;

    bool dir_deleted(uint32_t dir_pos) const;

    uint32_t get_dir_count() const;

private:
    uint32_t* dir_count;

    uint32_t* free_space;

    int32_t* dirs;
};
