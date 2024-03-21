#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "relational_model/schema.h"
#include "storage/heap_file/heap_file.h"

struct TableInfo {
    std::string name;
    std::unique_ptr<Schema> schema;
    std::unique_ptr<HeapFile> heap_file;

    TableInfo(const std::string name,
              std::unique_ptr<Schema> schema,
              std::unique_ptr<HeapFile> heap_file) :
        name      (name),
        schema    (std::move(schema)),
        heap_file (std::move(heap_file)) { }
};

class Catalog {
public:
    Catalog(const std::string& filename);

    ~Catalog();

    // may return nullptr if table does not exists
    // sets the schema when the table is found
    HeapFile* get_table(const std::string& table_name, Schema* schema);

    HeapFile* create_table(const std::string& table_name, const Schema&);

private:
    std::map<std::string, uint64_t> table_name_idx;

    std::vector<TableInfo> tables;

    std::fstream file;

    uint64_t read_uint64();

    std::string read_string();

    void write_uint64(const uint64_t);

    void write_string(const std::string&);
};
