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
    auto record_byte_len = RecordSerializer::get_size(record);

    uint32_t new_dir_pos = 0;
    while (!dir_deleted(new_dir_pos) && new_dir_pos < *dir_count) {
        new_dir_pos++;
    }

    if (new_dir_pos < *dir_count) { // found dir pos to reuse
        if (*free_space < record_byte_len)
            return false;

        *free_space -= record_byte_len;
    } else { // new_dir_pos == *dir_count
        if (*free_space < record_byte_len + sizeof(*dirs))
            return false;

        *dir_count += 1;
        *free_space -= record_byte_len + sizeof(*dirs);
    }

    // We add all the sizes in the definition of the HeapFile
    auto byte_offset = sizeof(*dir_count)
                    + sizeof(*free_space)
                    + (*dir_count) * sizeof(*dirs)
                    + (*free_space);
    dirs[new_dir_pos] = byte_offset;

    // We save the record
    RecordSerializer::serialize(record, page.data() + byte_offset);
    page.make_dirty();

    // We set the record
    *out_record_id = RID(page.page_id.page_number, new_dir_pos);
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
    dirs[dir_pos] = -1;
}


void HeapFilePage::vacuum(const Schema& schema) {
    uint32_t new_free_space = Page::SIZE - 2 * sizeof(uint32_t);
    uint32_t new_dir_count = 0;

    Record record_buf(schema.datatypes);
    char* page_buf = new char[Page::SIZE];
    auto new_dirs = reinterpret_cast<int32_t*>(page_buf+ 2*sizeof(uint32_t));

    for (uint32_t old_dir_pos = 0; old_dir_pos < *dir_count; old_dir_pos++) {
        if (dirs[old_dir_pos] > 0) {
            get_record(old_dir_pos, record_buf);
            auto record_byte_len = RecordSerializer::get_size(record_buf);

            new_dir_count++;
            new_free_space -= record_byte_len + sizeof(*dirs);

            auto byte_offset = sizeof(*dir_count)
                             + sizeof(*free_space)
                             + (new_dir_count) * sizeof(*dirs)
                             + (new_free_space);
            new_dirs[new_dir_count - 1] = byte_offset;
            RecordSerializer::serialize(record_buf, page_buf + byte_offset);
        }
    }
    std::memcpy(page.data(), page_buf, Page::SIZE);

    *dir_count = new_dir_count;
    *free_space = new_free_space;
    page.make_dirty();
    delete[] page_buf;
}
