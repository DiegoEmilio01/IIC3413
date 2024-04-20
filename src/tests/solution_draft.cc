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


bool HeapFilePage::try_insert_record(Record& record, RID* out_record_id) {
    auto record_byte_len = RecordSerializer::get_size(record);

    uint32_t pos;
    bool deletion_found = false;
    for (pos = 0; pos < *dir_count && deletion_found == false; pos++) {
        deletion_found = dir_deleted(pos); // We check if a record is deleted
    }

    // We prepare both types of insertions
    if (deletion_found && *free_space >= record_byte_len) {
        // -1 because the 'for' statement adds 1 before the condition check
        pos --;
        *free_space -= record_byte_len;
    } else if (*free_space >= record_byte_len + sizeof(*dirs)) {
        // We set the position at the end
        pos = *dir_count;
        *dir_count += 1;
        *free_space -= record_byte_len + sizeof(*dirs);
    } else {
        return false;
    }

    // We add all the sizes in the definition of the HeapFile
    auto byte_offset = sizeof(*dir_count)
                    + sizeof(*free_space)
                    + (*dir_count) * sizeof(*dirs)
                    + (*free_space);
    dirs[pos] = byte_offset;

    // We save the record
    char* write_ptr = page.data() + byte_offset;
    RecordSerializer::serialize(record, write_ptr);
    page.make_dirty();

    // We set the record
    *out_record_id = RID(page.page_id.page_number, pos);
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
    assert(dir_pos < *dir_count);
    if (! dir_deleted(dir_pos)) { // we dont delete if it is deleted
        dirs[dir_pos] = -1;
        page.make_dirty(); // the consequence of this command is costly
    }
}


void HeapFilePage::vacuum(const Schema& schema) {
    char buffer[Page::SIZE];
    auto buffer_dir_count  = reinterpret_cast<uint32_t*>(buffer);
    auto buffer_free_space = reinterpret_cast<uint32_t*>(buffer + sizeof(uint32_t));
    auto buffer_dirs       = reinterpret_cast<int32_t*> (buffer + 2*sizeof(uint32_t));

    *buffer_dir_count = 0;
    *buffer_free_space = Page::SIZE - 2*sizeof(uint32_t);
    auto datatypes = schema.datatypes;
    Record record_buf(std::move(datatypes));

    uint32_t new_pos = 0;
    for (uint32_t pos = 0; pos < *dir_count; pos++) {
        if (! dir_deleted(pos)){
            get_record(pos, record_buf);
            auto record_byte_len = RecordSerializer::get_size(record_buf);


            *buffer_free_space -= record_byte_len + sizeof(*dirs);
            *buffer_dir_count += 1;

            auto byte_offset = sizeof(*buffer_dir_count)
                    + sizeof(*buffer_free_space)
                    + (*buffer_dir_count) * sizeof(*dirs)
                    + (*buffer_free_space);
            buffer_dirs[new_pos] = byte_offset;

            // We save the record
            RecordSerializer::serialize(record_buf, &buffer[byte_offset]);
            new_pos++;
        }
    }

    memcpy(page.data(), buffer, Page::SIZE);
    page.make_dirty();
}
