#include "heap_file_page.h"

#include <cassert>

#include "relational_model/record.h"
#include "relational_model/record_serializer.h"
#include "relational_model/system.h"
#include "storage/page.h"

HeapFilePage::HeapFilePage(FileId file_id, uint64_t page_number) :
    page (buffer_mgr.get_page(file_id, page_number)) // get_page returns the page with a pin
{
    dir_count  = reinterpret_cast<uint32_t*>(page.data());
    free_space = reinterpret_cast<uint32_t*>(page.data() + sizeof(uint32_t));
    dirs       = reinterpret_cast<int32_t*> (page.data() + 2*sizeof(uint32_t));

    // if new page, initialize to be valid
    // new pages comes with all bytes setted at 0
    if (*dir_count == 0 && *free_space == 0) {
        *free_space = Page::SIZE - 2*sizeof(uint32_t);
        page.make_dirty();
    }
}


HeapFilePage::~HeapFilePage() {
    page.unpin();
}


uint32_t HeapFilePage::get_dir_count() const {
    return *dir_count;
}


bool HeapFilePage::try_insert_record(const Record& record, RID* out_record_id) {
    // TODO: not available until Lab1 extension is over
    // T2 branch will be updated at April 20th with this method
    return true;
}


void HeapFilePage::get_record(uint32_t dir_pos, Record& out) const {
    assert(dirs[dir_pos] > 0);
    char* write_ptr = page.data() + dirs[dir_pos];
    RecordSerializer::deserialize(write_ptr, out);
}


bool HeapFilePage::dir_deleted(uint32_t dir_pos) const {
    return dirs[dir_pos] <= 0;
}


void HeapFilePage::delete_record(uint32_t dir_pos) const {
    // TODO: not available until Lab1 extension is over
    // T2 branch will be updated at April 20th with this method
}


void HeapFilePage::vacuum(const Schema& schema) {
    // TODO: not available until Lab1 extension is over
    // T2 branch will be updated at April 20th with this method
}
