#include "catalog.h"

#include <stdexcept>

#include "exceptions/exceptions.h"
#include "relational_model/schema.h"
#include "relational_model/system.h"
#include "storage/heap_file/heap_file.h"
#include "storage/isam/isam_nonclustered.h"
#include "storage/isam/isam_nonclustered_dir.h"

using namespace std;

std::string Catalog::normalize(const std::string& table_name) {
    std::string res = table_name;
    std::transform(res.begin(),
                   res.end(),
                   res.begin(),
                   [](unsigned char c){ return std::tolower(c); }
    );

    for (char c : res) {
        if (c < 'a' && c > 'z' && c != '_') {
            throw QueryException("wrong table name: `"
                + table_name
                + "`. Only a-z letters and underscore ('_') are allowed.");
        }
    }
    return res;
}

uint64_t Catalog::get_table_pos(const std::string& table_name) const {
    std::string normalized_table_name = normalize(table_name);
    auto found = table_name_idx.find(normalized_table_name);
    if (found != table_name_idx.end()) {
        return found->second;
    } else {
        throw QueryException("table: `" + table_name + "` does not exist.");
    }
}



Catalog::Catalog(const string& filename) {
    auto file_path = file_mgr.get_file_path(filename);
    file.open(file_path, ios::out|ios::app);
    if (file.fail()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    file.close();
    file.open(file_path, ios::in|ios::out|ios::binary);

    file.seekg(0, file.end);
    if (file.tellg() == 0) {
        return; // catalog file is new
    }

    file.seekg(0, file.beg);

    uint64_t tables_count = read_uint64();
    for (uint64_t i = 0; i < tables_count; ++i) {
        std::vector<DataType> column_types;
        std::vector<std::string> column_names;

        std::string table_name = read_string();
        uint64_t table_cardinality = read_uint64();
        for (uint64_t c = 0; c < table_cardinality; ++c) {
            column_types.push_back(static_cast<DataType>(read_uint64()));
            column_names.push_back(read_string());
        }

        auto schema = std::make_unique<Schema>(
            std::move(column_names),
            std::move(column_types)
        );

        auto& schema_ref = *schema.get();
        auto heap_file = std::make_unique<HeapFile>(schema_ref, table_name);
        table_name_idx.insert({table_name, tables.size()});

        std::unique_ptr<Index> index;

        IndexType index_type = static_cast<IndexType>(read_uint64());
        switch (index_type) {
            case IndexType::NC_ISAM: {
                auto key_col_idx = read_uint64();
                index = std::make_unique<IsamNonClustered>(
                    *heap_file.get(),
                    key_col_idx,
                    normalize(table_name) + ".nc_isam"
                );
                break;
            }
            case IndexType::NONE:
                break;
        }

        tables.emplace_back(
            table_name,
            std::move(schema),
            std::move(heap_file),
            std::move(index)
        );
    }
}


Catalog::~Catalog() {
    file.seekg(0, file.beg);

    write_uint64(tables.size());
    for (auto& table_info : tables) {
        write_string(table_info.name);
        auto& schema = table_info.schema;

        assert(schema->column_names.size() == schema->datatypes.size());

        write_uint64(schema->column_names.size());
        for (size_t i = 0; i < schema->column_names.size(); i++) {
            write_uint64(static_cast<uint64_t>(schema->datatypes[i]));
            write_string(schema->column_names[i]);
        }

        // write index type
        if (table_info.index == nullptr) {
            write_uint64(static_cast<uint64_t>(IndexType::NONE));
        } else {
            auto index_type = table_info.index->get_type();
            write_uint64(static_cast<uint64_t>(index_type));
            switch (index_type) {
            case IndexType::NC_ISAM: {
                auto casted = reinterpret_cast<IsamNonClustered*>(table_info.index.get());
                write_uint64(casted->key_column_idx);
                break;
            }
            case IndexType::NONE:
                break;
        }
        }
    }

    file.close();
}


uint64_t Catalog::read_uint64() {
    uint64_t res = 0;
    uint8_t buf[8];
    file.read((char*)buf, sizeof(buf));

    for (int i = 0, shift = 0; i < 8; ++i, shift += 8) {
        res |= static_cast<uint64_t>(buf[i]) << shift;
    }

    if (!file.good()) {
        throw std::runtime_error("Error reading uint64");
    }

    return res;
}


string Catalog::read_string() {
    uint_fast32_t len = read_uint64();
    char* buf = new char[len];
    file.read(buf, len);
    string res(buf, len);
    delete[] buf;
    return res;
}


void Catalog::write_uint64(const uint64_t n) {
    uint8_t buf[8];
    for (unsigned int i = 0, shift = 0; i < sizeof(buf); ++i, shift += 8) {
        buf[i] = (n >> shift) & 0xFF;
    }
    file.write(reinterpret_cast<const char*>(buf), sizeof(buf));
}


void Catalog::write_string(const string& s) {
    write_uint64(s.size());
    file.write(s.c_str(), s.size());
}


HeapFile* Catalog::create_table(const std::string& table_name, const Schema& schema) {
    std::string normalized_table_name = normalize(table_name);

    auto found = table_name_idx.find(normalized_table_name);

    if (found != table_name_idx.end()) {
        throw QueryException("table: `" + table_name + "` already exists.");
    }

    table_name_idx.insert({normalized_table_name, tables.size()});

    auto heap_file = std::make_unique<HeapFile>(schema, normalized_table_name);

    tables.emplace_back(
        normalized_table_name,
        std::make_unique<Schema>(schema),
        std::move(heap_file),
        nullptr
    );

    return tables.back().heap_file.get();
}


HeapFile* Catalog::get_table(const std::string& table_name, Schema* schema) {
    std::string normalized_table_name = normalize(table_name);

    auto found = table_name_idx.find(normalized_table_name);

    if (found != table_name_idx.end()) {
        *schema = *tables[found->second].schema;
        return tables[found->second].heap_file.get();
    } else {
        return nullptr;
    }
}


void Catalog::insert_record(const std::string& table_name, const Record& record) {
    auto table_pos = get_table_pos(table_name);

    auto rid = tables[table_pos].heap_file->insert_record(record);

    auto index = tables[get_table_pos(table_name)].index.get();
    if (index != nullptr) {
        index->insert_record(rid);
    }
}


void Catalog::insert_record(
    const std::string& table_name,
    const std::vector<std::variant<std::string, int64_t>>& values)
{
    auto table_pos = get_table_pos(table_name);

    auto& record = *tables[table_pos].record_buf;
    record.set(values);

    auto rid = tables[table_pos].heap_file->insert_record(record);

    auto index = tables[get_table_pos(table_name)].index.get();
    if (index != nullptr) {
        index->insert_record(rid);
    }
}

void Catalog::delete_record(const std::string& table_name, RID rid) {
    auto table_pos = get_table_pos(table_name);

    // MUST delete from the index before the table, otherwise rid will be invalid
    auto index = get_index(table_name);
    if (index != nullptr) {
        index->delete_record(rid);
    }
    tables[table_pos].heap_file->delete_record(rid);
}


void Catalog::create_non_clustered_isam(const std::string& table_name, int key_col_idx) {
    auto table_pos = get_table_pos(table_name);

    if (tables[table_pos].index != nullptr) {
        throw QueryException("table: `" + table_name + "` already has an index.");
    }

    tables[table_pos].index = std::make_unique<IsamNonClustered>(
        *tables[table_pos].heap_file,
        key_col_idx,
        normalize(table_name) + ".nc_isam"
    );
}


Index* Catalog::get_index(const std::string& table_name) {
    return tables[get_table_pos(table_name)].index.get();
}


Record& Catalog::get_record_buf(const std::string& table_name) {
    return *tables[get_table_pos(table_name)].record_buf;
}


bool Catalog::table_exists(const std::string& table_name) const {
    std::string normalized_table_name = normalize(table_name);
    auto found = table_name_idx.find(normalized_table_name);
    return found != table_name_idx.end();
}


DataType Catalog::get_datatype(const std::string& table_name,
                               const std::string& col_name) const
{
    std::string normalized_table_name = normalize(table_name);
    auto found = table_name_idx.find(normalized_table_name);
    if (found == table_name_idx.end())
        throw QueryException("Table `" + table_name + "` does not exist");

    const auto& schema = tables[found->second].schema;

    for (size_t i = 0; i < schema->column_names.size(); i++) {
        if (schema->column_names[i] == col_name) {
            return schema->datatypes[i];
        }
    }

    throw QueryException("Column `" + col_name + "` does not exist in table `"
                         + table_name + "`");
}


const TableInfo& Catalog::get_table_info(const std::string& table_name) const {
    std::string normalized_table_name = normalize(table_name);
    return tables[get_table_pos(table_name)];
}


Index* Catalog::get_index_for_column(
    const std::string& table_name,
    const std::string& column_name)
{
    auto* index = get_index(table_name);
    if (index == nullptr) return nullptr;

    switch (index->get_type()) {
    case IndexType::NC_ISAM: {
        auto casted_isam = reinterpret_cast<IsamNonClustered*>(index);
        auto& indexed_column = casted_isam->heap_file.schema.column_names[casted_isam->key_column_idx];
        if (indexed_column == column_name) {
            return index;
        } else {
            return nullptr;
        }
    }
    default:
        return nullptr;
    }
}

FileId Catalog::get_file_id(TableId tid) {
    assert(tables.size() > tid);
    return tables[tid].heap_file->file_id;
}
