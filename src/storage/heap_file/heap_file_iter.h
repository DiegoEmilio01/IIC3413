#pragma once

#include <memory>

#include "relational_model/relation_iter.h"
#include "storage/heap_file/rid.h"

class HeapFile;
class HeapFilePage;

class HeapFileIter : public RelationIter {
public:
    HeapFileIter(const HeapFile& heap_file);

    virtual void begin(Record& out) override;

    virtual bool next() override;

    virtual void reset() override;

    RID get_current_RID() const;

private:
    const HeapFile& heap_file;

    std::unique_ptr<HeapFilePage> current_page;

    uint64_t total_pages;

    uint64_t current_page_number;

    int64_t current_page_record_pos;

    Record* out;
};
