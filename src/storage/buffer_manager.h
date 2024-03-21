#pragma once

#include <cstdint>
#include <mutex>
#include <unordered_map>

#include "storage/file_id.h"
#include "storage/page_id.h"

class Page;

class BufferManager {
public:
    static constexpr uint64_t DEFAULT_BUFFER_SIZE  = 1024 * 1024 * 1024; // 1 GB

    // buffer size in bytes
    BufferManager(uint64_t buffer_size);

    ~BufferManager();

    // Get a page. It will search in the buffer and if it is not on it, it will read from disk and put in the buffer.
    // Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the caller doesn't need
    // the returned page anymore.
    Page& get_page(FileId file_id, uint64_t page_number);

    // Similar to get_page, but the page_number is the smallest number such that page number does not exist on disk.
    // The page returned has all its bytes initialized to 0. This operation perform a disk write immediately
    // so 2 calls of append_page in a row will work as expected.
    Page& append_page(FileId file_id);

    // write all dirty pages to disk
    void flush();

private:
    // clock with second chance is used for page replacement
    uint64_t clock = 0;

    const uint64_t frame_count;

    Page* const frames;

    // allocated memory for the pages
    char* const data;

    // needed to avoid race conditions
    std::mutex pages_mutex;

    // used to search the index in the `buffer_pool` of a certain page
    // robin_hood::unordered_map<PageId, Page*> page_map;
    std::unordered_map<PageId, Page*> page_map;

    // returns the index of an unpinned page
    Page& get_unused_page();
};
