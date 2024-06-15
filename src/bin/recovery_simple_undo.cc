#include <fstream>
#include <iostream>

#include "relational_model/system.h"
#include "third_party/cli11/CLI11.hpp"


int main(int argc, char* argv[]) {
    std::string log_path;

    std::string db_directory;

    CLI::App app{"IIC 3413 DB"};
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("database", db_directory)
        ->description("Database directory")
        ->type_name("<path>")
        ->check(CLI::ExistingDirectory.description(""))
        ->required();

    app.add_option("log file", log_path)
        ->description("Log file")
        ->type_name("<path>")
        ->check(CLI::ExistingFile.description(""))
        ->required();

    CLI11_PARSE(app, argc, argv);

    std::fstream log_file(log_path, std::ios::binary|std::ios::in);

    if (log_file.fail()) {
        std::cerr << "Could not open the log at path: " << log_path << "\n";
        return EXIT_FAILURE;
    }

    // TODO: read the log and save useful info

    log_file.close();

    auto system = System::init(db_directory, BufferManager::DEFAULT_BUFFER_SIZE);

    // TODO: do the recovery with the info you got from the log
}
