#include "catalog.h"

#include <stdexcept>

#include "relational_model/schema.h"
#include "relational_model/system.h"
#include "storage/heap_file/heap_file.h"

using namespace std;

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

        tables.emplace_back(
            table_name,
            std::move(schema),
            std::move(heap_file)
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
    std::string normalized_table_name = table_name;

    std::transform(normalized_table_name.begin(),
                   normalized_table_name.end(),
                   normalized_table_name.begin(),
                   [](unsigned char c){ return std::tolower(c); }
    );

    for (char c : normalized_table_name) {
        if (c < 'a' && c > 'z' && c != '_') {
            throw std::invalid_argument("wrong table name: \""
                + table_name
                + "\". only a-z letters and underscore ('_') are allowed.");
        }
    }

    auto found = table_name_idx.find(normalized_table_name);

    if (found != table_name_idx.end()) {
        throw std::invalid_argument("table: \""
                + table_name
                + "\" already exists.");
    }

    table_name_idx.insert({table_name, tables.size()});

    auto heap_file = std::make_unique<HeapFile>(schema, normalized_table_name);
    table_name_idx.insert({table_name, tables.size()});

    tables.emplace_back(
        table_name,
        std::make_unique<Schema>(schema),
        std::move(heap_file)
    );

    return tables.back().heap_file.get();
}


HeapFile* Catalog::get_table(const std::string& table_name, Schema* schema) {
    std::string normalized_table_name = table_name;

    std::transform(normalized_table_name.begin(),
                   normalized_table_name.end(),
                   normalized_table_name.begin(),
                   [](unsigned char c){ return std::tolower(c); }
    );

    for (char c : normalized_table_name) {
        if (c < 'a' && c > 'z' && c != '_') {
            throw std::invalid_argument("wrong table name: \""
                + table_name
                + "\". only a-z letters and underscore ('_') are allowed.");
        }
    }

    auto found = table_name_idx.find(normalized_table_name);

    if (found != table_name_idx.end()) {
        *schema = *tables[found->second].schema;
        return tables[found->second].heap_file.get();
    } else {
        return nullptr;
    }
}
