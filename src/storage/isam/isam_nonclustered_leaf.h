
#pragma once

#include "storage/isam/isam_nonclustered.h"


class IsamNonClusteredLeaf {
friend class IsamNonClustered;
friend class IsamNonClusteredIter;

struct RecordInfo {
    int64_t key;
    RID rid;

    RecordInfo(int64_t key, RID rid) : key(key), rid(rid) {}
};

public:
    IsamNonClusteredLeaf(const IsamNonClustered& isam, uint64_t page_number);

    // this constructor creates a new page
    IsamNonClusteredLeaf(const IsamNonClustered& isam);

    ~IsamNonClusteredLeaf();

    void insert_record(RID rid, int64_t key);

    void delete_record(RID rid);

private:
    const IsamNonClustered& isam;

    Page& page;

    uint32_t* N;

    int32_t* prev;

    int32_t* next;

    int32_t* overflow;

    RecordInfo* records;

public:
    static constexpr auto max_keys = (Page::SIZE - sizeof(*N) - sizeof(*prev) - sizeof(*next) - sizeof(*overflow)) / (sizeof(*records));

    static constexpr auto max_space_used = sizeof(*N) + sizeof(*prev) + sizeof(*next) + sizeof(*overflow) + max_keys * sizeof(*records);

    static_assert(max_space_used <= Page::SIZE);
};
