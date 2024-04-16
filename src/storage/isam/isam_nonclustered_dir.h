#pragma once

#include "storage/isam/isam_nonclustered.h"

class IsamNonClusteredDir {
friend class IsamNonClustered;
public:
    IsamNonClusteredDir(const IsamNonClustered& isam, uint64_t page_number);

    ~IsamNonClusteredDir();

    void insert_record(RID rid, int64_t key);

    void delete_record(RID rid, int64_t key);

    // returns the leaf page number
    int_fast32_t search_leaf(int64_t encoded_key);

private:
    int_fast32_t search_child_idx(int64_t key);

    const IsamNonClustered& isam;

    Page& page;

    // first 8 bytes is N, with 0 < N <= max_keys
    // N keys (8 bytes)
    // (N+1) ptrs (4 bytes)
    uint64_t* N; // (we use 8 bytes for alignment of keys, could be 4 bytes)

    int64_t* keys;

    int32_t* children;

public:
    static constexpr auto max_keys = (Page::SIZE - sizeof(*N) - sizeof(*children)) / (sizeof(*keys)+sizeof(*children));

    static constexpr auto max_space_used = sizeof(*N) + max_keys * sizeof(*keys) + (max_keys+1) * (sizeof(*children));

    static_assert(max_space_used <= Page::SIZE);
};
