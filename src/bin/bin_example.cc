#include <iostream>

#include "relational_model/system.h"
#include "third_party/cli11/CLI11.hpp"

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);

    uint64_t buffer_size  = BufferManager::DEFAULT_BUFFER_SIZE;
    std::string db_directory;
    std::string command;

    CLI::App app{"IIC 3413 DB"};
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("database", db_directory)
        ->description("Database directory")
        ->type_name("<path>")
        ->check(CLI::ExistingDirectory.description(""))
        ->required();

    app.add_option("--buffer", buffer_size)
        ->description("Size of buffer\nAllows units such as MB and GB")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    CLI11_PARSE(app, argc, argv);

    // Need to call System::init before start using the database
    // When this object comes out of scope the database is no longer usable
    auto system = System::init(db_directory, buffer_size);

    std::cout << "IIC 3413 DB initialized\n";
    std::cout << "  database directory: " << db_directory << std::endl;

    // TODO: we will do useful things in the next homeworks
    // for now we just end the program

    return EXIT_SUCCESS;
}
