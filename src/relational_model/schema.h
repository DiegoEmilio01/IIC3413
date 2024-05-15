#pragma once

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

// IIC3413-DB will only support as datatypes:
// - strings up to 255 bytes length in UTF-8 representation
// - 64 bit signed integer
enum class DataType {
    STR,
    INT
};


class Schema {
public:
    Schema(std::vector<std::string>&& column_names,
           std::vector<DataType>&&    datatypes) :
        column_names (std::move(column_names)),
        datatypes    (std::move(datatypes))
    {
        assert(column_names.size() == datatypes.size());
    }

    Schema() {}

    std::vector<std::string> column_names;

    std::vector<DataType> datatypes;

    bool operator==(const Schema& other) const {
        return datatypes == other.datatypes && column_names == other.column_names;
    }
};
