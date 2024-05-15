#pragma once

#include <cstdint>
#include <string>
#include <ostream>

#include "relational_model/schema.h"

union ValueUnion {
    char* as_str;
    int64_t as_int;

    explicit ValueUnion(char* str_value) : as_str(str_value) {}
    explicit ValueUnion(int64_t int_value) : as_int(int_value) {}
};

class Value {
public:
    explicit Value(int64_t);
    explicit Value(const char* str_value);
    explicit Value(const std::string&);

    Value(Value&& other);
    Value(const Value& other);

    ~Value();

    void operator=(const Value& other);
    void operator=(Value&& other);

    bool operator <(const Value& other) const;
    bool operator ==(const Value& other) const;

    bool operator !=(const Value& other) const {
        return !(*this == other);
    }

    bool operator <=(const Value& other) const {
        return !(other < *this);
    }

    bool operator >=(const Value& other) const {
        return !(*this < other);
    }

    bool operator >(const Value& other) const {
        return other < *this;
    }

    DataType datatype;
    ValueUnion value;

    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        switch (v.datatype) {
        case DataType::INT: {
            os << v.value.as_int;
            break;
        }
        case DataType::STR:
            os << '"' << v.value.as_str << '"';
            break;
        }
        return os;
    }
};