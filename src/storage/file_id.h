#pragma once

#include <cstdint>

class FileId {
public:
    static constexpr int UNASSIGNED = INT32_MAX;

    int id;

    FileId(int id) : id(id) { }

    bool operator<(const FileId other) const {
        return this->id < other.id;
    }

    bool operator==(const FileId other) const {
        return this->id == other.id;
    }
};
