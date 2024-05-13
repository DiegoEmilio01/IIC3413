#include <iostream>

#include "relational_model/system.h"

constexpr uint64_t GB = 1024 * 1024 * 1024; // 1 GB

int main() {
    uint64_t buffer_size = 1 * GB;
    std::string database_folder = "data/test_example";

    // Need to call System::init before start using the database
    // When this object comes out of scope the database is no longer usable
    auto system = System::init(database_folder, buffer_size);

    Schema table1_schema(
        { "col1", "col2"},
        { DataType::INT, DataType::STR }
    );

    Schema existing_table_schema;

    std::string table1 = "test_5";
    HeapFile* table = catalog.get_table(table1, &existing_table_schema);

    if (table == nullptr) { // table doesn't exist
        table = catalog.create_table(table1, table1_schema);
        for (int i = 0; i < 50000; i++) {
            catalog.insert_record(table1, {i, "record_" + std::to_string(i)});
        }

        catalog.create_non_clustered_isam(table1, 0);

    } else {
        assert(existing_table_schema == table1_schema);
    }

    return 0;
}
