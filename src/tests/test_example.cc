#include <iostream>

#include "relational_model/record.h"
#include "relational_model/relation_iter.h"
#include "relational_model/schema.h"
#include "relational_model/system.h"

constexpr uint64_t GB = 1024 * 1024 * 1024; // 1 GB

int main() {
    uint64_t buffer_size = 1 * GB;
    std::string database_folder = "data/test_example";

    // Need to call System::init before start using the database
    // When this object comes out of scope the database is no longer usable
    auto system = System::init(database_folder, buffer_size);

    Schema table1_schema(
        { "col1", "col2" },
        { DataType::STR, DataType::INT }
    );

    Schema existing_table_schema;
    HeapFile* table = catalog.get_table("test1", &existing_table_schema);

    if (table == nullptr) { // table doesn't exist
        table = catalog.create_table("test1", table1_schema);
    } else {
        assert(existing_table_schema == table1_schema);
    }

    Record record_buf({DataType::STR, DataType::INT});

    record_buf.set({"test_record1", 1});
    table->insert_record(record_buf);

    record_buf.set({"test_record2", 2});
    table->insert_record(record_buf);

    record_buf.set({"test_record3", 3});
    auto r3 = table->insert_record(record_buf);

    record_buf.set({"test_record4", 4});
    table->insert_record(record_buf);

    table->delete_record(r3);

    auto table_iter = table->get_record_iter();

    table_iter->begin(record_buf);

    while (table_iter->next()) {
        // print result
        std::cout << record_buf << "\n";
    }

    return 0;
}
