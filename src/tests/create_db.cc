#include <iostream>

#include "query/parser/parser.h"
#include "relational_model/system.h"

constexpr uint64_t GB = 1024 * 1024 * 1024; // 1 GB

int main() {
    uint64_t buffer_size = 1 * GB;
    std::string database_folder = "data/test_example";

    // Need to call System::init before start using the database
    // When this object comes out of scope the database is no longer usable
    auto system = System::init(database_folder, buffer_size);

    Schema table1_schema(
        { "a", "b" },
        { DataType::STR, DataType::INT }
    );

    Schema table2_schema(
        { "a", "b", "c" },
        { DataType::INT, DataType::INT, DataType::INT }
    );

    Schema existing_table_schema;

    std::string table1_name = "T1";
    std::string table2_name = "T2";

    HeapFile* table1 = catalog.get_table(table1_name, &existing_table_schema);
    if (table1 == nullptr) { // table doesn't exist
        table1 = catalog.create_table(table1_name, table1_schema);
        for (int i = 0; i < 10; i++) {
            catalog.insert_record(table1_name, {"test.record." + std::to_string(i), i+30});
            catalog.insert_record(table1_name, {"test.not.record." + std::to_string(i), 10*i+30});
        }

        catalog.create_non_clustered_isam(table1_name, 0); // 0 represents column a

    } else {
        assert(existing_table_schema == table1_schema);
    }

    HeapFile* table2 = catalog.get_table(table2_name, &existing_table_schema);
    if (table2 == nullptr) { // table doesn't exist
        table2 = catalog.create_table(table2_name, table2_schema);
        for (int i = 0; i < 10; i++) {
            catalog.insert_record(table2_name, {i+1, i+5, i*2-1});
        }

        catalog.create_non_clustered_isam(table2_name, 2); // 2 represents column c

    } else {
        assert(existing_table_schema == table2_schema);
    }

    return 0;
}
