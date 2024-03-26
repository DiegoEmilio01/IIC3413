#include "heap_file_iter.h"

#include "relational_model/record_serializer.h"
#include "relational_model/system.h"
#include "storage/heap_file/heap_file.h"
#include "storage/heap_file/heap_file_page.h"


HeapFileIter::HeapFileIter(HeapFile& heap_file) :
    heap_file (heap_file)
{
    // value starts as -1 because in next we always sum 1 before processing
    current_page_record_pos = -1;

    current_page_number = 0;
    current_page = std::make_unique<HeapFilePage>(heap_file.file_id, 0);
    total_pages = file_mgr.count_pages(heap_file.file_id);
}


void HeapFileIter::begin(Record& out) {
    this->out = &out;
}


bool HeapFileIter::next() {

    while (current_page != nullptr) {
        current_page_record_pos++;
        // static_cast in not necessary, only to avoid a warning
        if (current_page_record_pos >= static_cast<int64_t>(current_page->get_dir_count())) {
            current_page_record_pos = -1;
            current_page_number++;

            if (current_page_number < total_pages) {
                current_page = std::make_unique<HeapFilePage>(
                    heap_file.file_id, current_page_number
                );
                continue;
            } else {
                current_page = nullptr;
                return false;
            }
        }

        if (current_page->dir_deleted(current_page_record_pos)) {
            continue;
        }

        current_page->get_record(current_page_record_pos, *out);
        return true;
    }
    return false;
}


void HeapFileIter::reset() {
    current_page_record_pos = -1;
    current_page_number = 0;
    current_page = std::make_unique<HeapFilePage>(heap_file.file_id, 0);
}
