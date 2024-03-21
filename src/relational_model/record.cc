#include "record.h"

#include <cassert>
#include <cstring>

Record::Record(std::vector<DataType>&& _types) :
    types (std::move(_types))
{
    for (auto type : types) {
        switch (type) {
        case DataType::INT: {
            values.push_back(OutValue((int64_t) 0));
            break;
        }
        case DataType::STR: {
            char* str_buffer = new char[MAX_STRLEN+1];
            std::memset(str_buffer, '\0', MAX_STRLEN+1);
            values.push_back(OutValue(str_buffer));
            break;
        }
        }
    }
}


Record::~Record() {
    auto index = 0;
    for (auto type : types) {
        switch (type) {
        case DataType::INT: {
            // no need to free
            break;
        }
        case DataType::STR: {
            delete[] values[index].str_value;
            break;
        }
        }
        index++;
    }
}

void Record::set(const std::vector<std::variant<std::string, int64_t>>& new_values) {
    assert(new_values.size() == values.size());
    assert(new_values.size() == types.size());

    for (size_t i = 0; i < values.size(); i++) {
        auto& value = new_values[i];

        switch (types[i]) {
        case DataType::INT: {
            assert(std::holds_alternative<int64_t>(value));
            values[i].int_value = std::get<int64_t>(value);
            break;
        }
        case DataType::STR: {
            assert(std::holds_alternative<std::string>(value));
            auto& str = std::get<std::string>(value);
            assert(str.size() <= Record::MAX_STRLEN);
            memcpy(values[i].str_value, str.c_str(), str.size()+1);
            break;
        }
        }
    }
}
