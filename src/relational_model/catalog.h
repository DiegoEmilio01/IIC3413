#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "relational_model/index.h"
#include "relational_model/schema.h"
#include "storage/heap_file/heap_file.h"

struct TableInfo {
    std::string name;
    std::unique_ptr<Schema> schema;
    std::unique_ptr<HeapFile> heap_file;

    // used as buffer, to avoid allocations
    std::unique_ptr<Record> record_buf;

    // nullptr if there is not index
    std::unique_ptr<Index> index;

    TableInfo(const std::string name,
              std::unique_ptr<Schema> _schema,
              std::unique_ptr<HeapFile> heap_file,
              std::unique_ptr<Index> index) :
        name      (name),
        schema    (std::move(_schema)),
        heap_file (std::move(heap_file)),
        index     (std::move(index))
    {
        record_buf = std::make_unique<Record>(schema->datatypes);
    }
};


class Catalog {
public:
    Catalog(const std::string& filename);

    ~Catalog();

    // may return nullptr if table does not exists
    // sets the schema when the table is found
    HeapFile* get_table(const std::string& table_name, Schema* schema);

    HeapFile* create_table(const std::string& table_name, const Schema&);

    void insert_record(const std::string& table_name, const Record& record);

    void insert_record(
        const std::string& table_name,
        const std::vector<std::variant<std::string, int64_t>>& values);

    void delete_record(const std::string& table_name, RID rid);

    void create_non_clustered_isam(const std::string& table_name, int key_col_idx);

    Index* get_index(const std::string& table_name);

    Record& get_record_buf(const std::string& table_name);

private:
    std::map<std::string, uint64_t> table_name_idx;

    std::vector<TableInfo> tables;

    std::fstream file;

    uint64_t read_uint64();

    std::string read_string();

    uint64_t get_table_pos(const std::string& table_name);

    void write_uint64(const uint64_t);

    void write_string(const std::string&);
};
