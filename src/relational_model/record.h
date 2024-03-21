#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>
#include <variant>
#include <vector>

#include "relational_model/schema.h"

union OutValue {
    char* str_value;
    int64_t int_value;

    explicit OutValue(char* str_value) : str_value(str_value) {}
    explicit OutValue(int64_t int_value) : int_value(int_value) {}
};

class Record {
public:
    static constexpr int64_t MAX_STRLEN = 255;

    Record(std::vector<DataType>&& types);

    ~Record();

    void set(const std::vector<std::variant<std::string, int64_t>>& values);

    friend std::ostream& operator<<(std::ostream& os, const Record& o) {
        assert(o.types.size() == o.values.size());

        char separator[2] = {'\0', '\0'};
        for (unsigned i = 0; i < o.types.size(); i++) {
            os << separator;
            switch (o.types[i]) {
            case DataType::INT: {
                os << o.values[i].int_value;
                break;
            }
            case DataType::STR:
                os << '"' << o.values[i].str_value << '"';
                break;
            }
            separator[0] = ',';
        }
        return os;
    }

    std::vector<OutValue> values;

    std::vector<DataType> types;
};
