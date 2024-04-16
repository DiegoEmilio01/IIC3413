#include "heap_file.h"

#include "relational_model/record_serializer.h"
#include "relational_model/system.h"
#include "storage/heap_file/heap_file_iter.h"
#include "storage/heap_file/heap_file_page.h"

HeapFile::HeapFile(const Schema& schema, const std::string& table_name) :
    schema (schema),
    file_id (file_mgr.get_file_id(table_name)) { }


RID HeapFile::insert_record(const Record& record) {
    // buffer_mgr.get_page will pin the page
    auto current_page = std::make_unique<HeapFilePage>(
        file_id, last_insert_page
    );
    RID res;

    // search block with available space and insert it there
    while (true) {
        if (current_page->try_insert_record(record, &res)) {
            return res;
        }
        last_insert_page++;
        current_page = std::make_unique<HeapFilePage>(
            file_id, last_insert_page
        );
    }
}


std::unique_ptr<HeapFileIter> HeapFile::get_record_iter() const {
    return std::make_unique<HeapFileIter>(*this);
}


void HeapFile::delete_record(RID rid) {
    HeapFilePage page(file_id, rid.page_num);
    page.delete_record(rid.dir_slot);
}


void HeapFile::vacuum() {
    uint64_t total_pages = file_mgr.count_pages(file_id);

    for (uint64_t i = 0; i < total_pages; i++) {
        HeapFilePage page(file_id, i);
        page.vacuum(schema);
    }
    last_insert_page = 0;
}

void HeapFile::get_record(RID rid, Record& out) const {
    HeapFilePage page(file_id, rid.page_num);
    page.get_record(rid.dir_slot, out);
}