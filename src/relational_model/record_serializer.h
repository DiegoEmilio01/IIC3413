#pragma once

#include <cstddef>

class Record;

class RecordSerializer {
public:
    // gets the size necessary for the serialization of a tuple
    static size_t get_size(const Record& in);

    // `out` needs to point into allocated memory of at least `get_size(out)` bytes
    static void serialize(const Record& in, char* out);

    static void deserialize(const char* in, Record& out);
};
