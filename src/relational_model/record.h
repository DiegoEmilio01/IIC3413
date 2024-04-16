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
            switch (o.values[i].datatype) {
            case DataType::INT: {
                os << o.values[i].value.as_int;
                break;
            }
            case DataType::STR:
                os << '"' << o.values[i].value.as_str << '"';
                break;
            }
            separator[0] = ',';
        }
        return os;
    }

    std::vector<Value> values;
};
