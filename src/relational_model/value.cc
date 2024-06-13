#include "value.h"

#include <cstring>

#include "relational_model/record.h"

Value::Value(Value&& other) :
    datatype(other.datatype), value(other.value)
{
    if (datatype == DataType::STR) {
        // change datatype to avoid deleting value.as_str when other is destroyed
        other.datatype = DataType::INT;
    }
}


Value::Value(const Value& other) :
    datatype(other.datatype), value(other.value)
{
    if (datatype == DataType::STR) {
        value.as_str = new char[Record::MAX_STRLEN+1];
        std::memcpy(value.as_str, other.value.as_str, Record::MAX_STRLEN+1);
    }
}

void Value::operator=(const Value& other) {
    if (other.datatype == DataType::STR) {
        if (datatype != DataType::STR) {
            value.as_str = new char[Record::MAX_STRLEN+1];
        }
        std::memcpy(value.as_str, other.value.as_str, Record::MAX_STRLEN+1);
    } else {
        if (datatype == DataType::STR) {
            delete[] value.as_str;
        }
        this->value = other.value;
    }
    this->datatype = other.datatype;
}


void Value::operator=(Value&& other) {
    if (datatype == DataType::STR) {
        delete[] value.as_str;
    }

    this->datatype = other.datatype;
    this->value    = other.value;
    if (datatype == DataType::STR) {
        // change datatype to avoid deleting value.as_str when other is destroyed
        other.datatype = DataType::INT;
    }
}


Value::Value(int64_t i) :
    datatype(DataType::INT), value(i) { }


Value::Value(const char* str_value) :
    datatype(DataType::STR),
    value(nullptr)
{
    value.as_str = new char[Record::MAX_STRLEN+1];

    auto size = strlen(str_value);
    assert(size <= Record::MAX_STRLEN);
    std::memcpy(value.as_str, str_value, size+1); // size+1 to copy '\0'
}


Value::Value(const std::string& str_value) :
    datatype(DataType::STR),
    value(nullptr)
{
    value.as_str = new char[Record::MAX_STRLEN+1];

    auto size = str_value.size();
    assert(size <= Record::MAX_STRLEN);
    std::memcpy(value.as_str, str_value.c_str(), size+1); // size+1 to copy '\0'
}


Value::~Value() {
    if (datatype == DataType::STR) {
        delete[] value.as_str;
    }
}


bool Value::operator<(const Value& other) const {
    if (datatype != other.datatype)
        return datatype < other.datatype;

    switch (datatype) {
        case DataType::INT: {
            return value.as_int < other.value.as_int;
        }
        case DataType::STR: {
            return std::strcmp(value.as_str, other.value.as_str) < 0;
        }
    }
    assert(false);
    return false; // unreachable
}


bool Value::operator==(const Value& other) const {
    if (datatype != other.datatype)
        return false;

    switch (datatype) {
        case DataType::INT: {
            return value.as_int == other.value.as_int;
        }
        case DataType::STR: {
            return std::strcmp(value.as_str, other.value.as_str) == 0;
        }
    }
    assert(false);
    return false; // unreachable
}
