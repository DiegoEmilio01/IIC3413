#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>
#include <variant>
#include <vector>

#include "relational_model/schema.h"
#include "relational_model/value.h"

class Record {
public:
    static constexpr int64_t MAX_STRLEN = 255;

    Record(const std::vector<DataType>& types);

    Record(const Record& other) = delete;

    void set(const std::vector<std::variant<std::string, int64_t>>& values);

    friend std::ostream& operator<<(std::ostream& os, const Record& o) {
        char separator[2] = {'\0', '\0'};
        for (unsigned i = 0; i < o.values.size(); i++) {
            os << separator;
            os << o.values[i];
            separator[0] = ',';
        }
        return os;
    }

    std::vector<Value> values;
};

class RecordRef {
public:
    RecordRef(uint_fast32_t size) {
        values.resize(size);
    }

    RecordRef(const RecordRef& other) = delete;

    friend std::ostream& operator<<(std::ostream& os, const RecordRef& o) {
        char separator[2] = {'\0', '\0'};
        for (unsigned i = 0; i < o.values.size(); i++) {
            os << separator;
            os << *o.values[i];
            separator[0] = ',';
        }
        return os;
    }

    std::vector<Value*> values;
};
