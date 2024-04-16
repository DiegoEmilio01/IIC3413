#pragma once

#include <cstdint>

struct RID {
    uint32_t page_num;
    uint32_t dir_slot;

    RID() {}

    RID(uint32_t page_num, uint32_t dir_slot) :
        page_num(page_num), dir_slot(dir_slot) {}

    bool operator==(const RID& other) {
        return page_num == other.page_num && dir_slot == other.dir_slot;
    }
};
