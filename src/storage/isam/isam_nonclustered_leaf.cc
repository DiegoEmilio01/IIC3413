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
    // TODO: implement
}


void IsamNonClusteredLeaf::IsamNonClusteredLeaf::delete_record(RID rid) {
    // TODO: implement
}
