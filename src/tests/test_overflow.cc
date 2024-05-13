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
        { "col1", "col2", "col3" },
        { DataType::INT, DataType::STR, DataType::INT }
    );

    Schema existing_table_schema;

    std::string table1 = "test_4";
    HeapFile* table = catalog.get_table(table1, &existing_table_schema);

    if (table == nullptr) { // table doesn't exist
        table = catalog.create_table(table1, table1_schema);
        for (int i = 0; i < 1000; i++) {
            catalog.insert_record(table1, {i, "record___" + std::to_string(i), i});
        }

        catalog.create_non_clustered_isam(table1, 1);

    } else {
        assert(existing_table_schema == table1_schema);
    }

    auto isam = catalog.get_index(table1);
    assert(isam != nullptr);

    catalog.delete_record(table1, RID(3,10)); // id 383

    return 0;
}
