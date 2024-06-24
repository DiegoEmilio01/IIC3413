#include <iostream>

#include "query/parser/parser.h"
#include "relational_model/system.h"

constexpr uint64_t GB = 1024 * 1024 * 1024; // 1 GB

int main() {
    std::string database_folder = "data/test_example";

    // Need to call System::init before start using the database
    // When this object comes out of scope the database is no longer usable
    auto system = System::init(database_folder, 1 * GB);

    Schema table1_schema(
        { "a", "b", "c" },
        { DataType::INT, DataType::INT, DataType::INT }
    );

    Schema table2_schema(
        { "x", "y", "z" },
        { DataType::INT, DataType::INT, DataType::INT }
    );

    Schema table3_schema(
        { "u", "v" },
        { DataType::INT, DataType::INT }
    );

    Schema existing_table_schema;

    std::string table1_name = "R";
    std::string table2_name = "S";
    std::string table3_name = "T";

    HeapFile* table1 = catalog.get_table(table1_name, &existing_table_schema);
    if (table1 == nullptr) { // table doesn't exist
        table1 = catalog.create_table(table1_name, table1_schema);
        for (int i = 0; i < 10; i++) {
            catalog.insert_record(table1_name, {i, 2*i, 3*i});
        }
    } else {
        assert(existing_table_schema == table1_schema);
    }

    HeapFile* table2 = catalog.get_table(table2_name, &existing_table_schema);
    if (table2 == nullptr) { // table doesn't exist
        table2 = catalog.create_table(table2_name, table2_schema);
        for (int i = 0; i < 10; i++) {
            catalog.insert_record(table2_name, {i, i*i, i*i*i});
        }
    } else {
        assert(existing_table_schema == table2_schema);
    }

    HeapFile* table3 = catalog.get_table(table3_name, &existing_table_schema);
    if (table3 == nullptr) { // table doesn't exist
        table3 = catalog.create_table(table3_name, table3_schema);
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                catalog.insert_record(table3_name, {i, j});
            }
        }
    } else {
        assert(existing_table_schema == table3_schema);
    }

    return 0;
}