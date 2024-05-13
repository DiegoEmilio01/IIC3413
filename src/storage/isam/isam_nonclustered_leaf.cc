#include "isam_nonclustered_leaf.h"

#include "relational_model/system.h"

// constructor for existing page
IsamNonClusteredLeaf::IsamNonClusteredLeaf(
    const IsamNonClustered& isam,
    uint64_t page_number
) :
    isam (isam),
    page (buffer_mgr.get_page(isam.leaf_file_id, page_number))
{
    N = reinterpret_cast<uint32_t*>(page.data());

    prev = reinterpret_cast<int32_t*>(page.data() + sizeof(*N));

    next = reinterpret_cast<int32_t*>(page.data() + sizeof(*N) + sizeof(*prev));

    overflow = reinterpret_cast<int32_t*>(
        page.data() + sizeof(*N) + sizeof(*prev) + sizeof(*next)
    );

    records = reinterpret_cast<RecordInfo*>(
        page.data() + sizeof(*N) + sizeof(*prev) + sizeof(*next) + sizeof(*overflow)
    );
}


// constructor for new page
IsamNonClusteredLeaf::IsamNonClusteredLeaf(
    const IsamNonClustered& isam
) :
    isam (isam),
    page (buffer_mgr.append_page(isam.leaf_file_id))
{
    N = reinterpret_cast<uint32_t*>(page.data());

    prev = reinterpret_cast<int32_t*>(page.data() + sizeof(*N));

    next = reinterpret_cast<int32_t*>(page.data() + sizeof(*N) + sizeof(*prev));

    overflow = reinterpret_cast<int32_t*>(
        page.data() + sizeof(*N) + sizeof(*prev) + sizeof(*next)
    );

    records = reinterpret_cast<RecordInfo*>(
        page.data() + sizeof(*N) + sizeof(*prev) + sizeof(*next) + sizeof(*overflow)
    );

    *N = 0;
    *prev = -1;
    *next = -1;
    *overflow = -1;
}


IsamNonClusteredLeaf::~IsamNonClusteredLeaf() {
    assert(page.get_page_number() != static_cast<uint32_t>(*overflow));
    page.unpin();
}


void IsamNonClusteredLeaf::insert_record(RID rid, int64_t key) {
    if (*N < max_keys) {
        records[*N] = RecordInfo(key, rid);
        *N += 1;
        page.make_dirty();
    } else {
        if (*overflow == -1) {
            IsamNonClusteredLeaf overflow_page(isam);
            *overflow = overflow_page.page.page_id.page_number;
            page.make_dirty();
            overflow_page.insert_record(rid, key);
        } else {
            IsamNonClusteredLeaf overflow_page(isam, *overflow);
            overflow_page.insert_record(rid, key);
        }
    }
}


void IsamNonClusteredLeaf::IsamNonClusteredLeaf::delete_record(RID rid) {
    // search in this leaf
    for (uint_fast32_t i = 0; i < *N; i++) {
        if (records[i].rid == rid) {
            // shift left
            for (uint_fast32_t j = i; j < (*N) - 1; j++) {
                records[j] = records[j+1];
            }
            *N -= 1;
            page.make_dirty();
            return;
        }
    }

    int32_t next_overflow = *overflow;
    while (next_overflow != -1) {
        IsamNonClusteredLeaf overflow_page(isam, next_overflow);

        // search in this leaf
        for (uint_fast32_t i = 0; i < *overflow_page.N; i++) {
            if (overflow_page.records[i].rid == rid) {
                // shift left
                for (uint_fast32_t j = i; j < (*overflow_page.N) - 1; j++) {
                    overflow_page.records[j] = overflow_page.records[j+1];
                }
                *overflow_page.N -= 1;
                overflow_page.page.make_dirty();
                return;
            }
        }
        next_overflow = *overflow_page.overflow;
    }
}
