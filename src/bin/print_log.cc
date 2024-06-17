#include <fstream>
#include <iostream>

#include "relational_model/system.h"
#include "third_party/cli11/CLI11.hpp"

uint32_t read_uint32(std::fstream& log_file) {
    uint32_t res;

    char small_buffer[4];
    log_file.read(small_buffer, 4);

    char* res_bytes = reinterpret_cast<char*>(&res);

    res_bytes[0] = small_buffer[0];
    res_bytes[1] = small_buffer[1];
    res_bytes[2] = small_buffer[2];
    res_bytes[3] = small_buffer[3];

    return res;
}


int main(int argc, char* argv[]) {
    std::string log_path;

    CLI::App app{"IIC 3413 DB"};
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

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

    char* buffer = new char[Page::SIZE];

    log_file.read(buffer, 1);
    while (log_file.good()) {
        std::cout << "<";

        LogType log_type = static_cast<LogType>(buffer[0]);
        switch (log_type) {
        case LogType::START: {
            auto tid = read_uint32(log_file);

            std::cout << "START," << tid;
            break;
        }
        case LogType::COMMIT: {
            auto tid = read_uint32(log_file);

            std::cout << "COMMIT," << tid;
            break;
        }
        case LogType::ABORT: {
            auto tid = read_uint32(log_file);

            std::cout << "ABORT," << tid;
            break;
        }
        case LogType::END: {
            auto tid = read_uint32(log_file);

            std::cout << "END," << tid;
            break;
        }
        case LogType::WRITE_UR: {
            auto tid = read_uint32(log_file);
            auto table_id = read_uint32(log_file);
            auto page_num = read_uint32(log_file);
            auto offset = read_uint32(log_file);
            auto len = read_uint32(log_file);

            std::cout << "WRITE-UR," << tid << ',' << table_id << ',' << page_num << ',' << offset << ',' << len << ',';

            std::cout << "0x";
            std::cout << std::hex;
            log_file.read(buffer, len);
            for (size_t i = 0; i < len; i++) {
                std::cout << std::setw(2) << std::setfill('0') << (int) static_cast<uint8_t>(buffer[i]);
            }

            std::cout << ',';

            std::cout << "0x";
            log_file.read(buffer, len);
            for (size_t i = 0; i < len; i++) {
                std::cout << std::setw(2) << std::setfill('0') << (int) static_cast<uint8_t>(buffer[i]);
            }
            std::cout << std::dec;

            break;
        }
        case LogType::WRITE_U: {
            auto tid = read_uint32(log_file);
            auto table_id = read_uint32(log_file);
            auto page_num = read_uint32(log_file);
            auto offset = read_uint32(log_file);
            auto len = read_uint32(log_file);

            std::cout << "WRITE-U," << tid << ',' << table_id << ',' << page_num << ',' << offset << ',' << len << ',';

            std::cout << std::hex;
            log_file.read(buffer, len);
            std::cout << "0x";
            for (size_t i = 0; i < len; i++) {
                std::cout << std::setw(2) << std::setfill('0') << (int) static_cast<uint8_t>(buffer[i]);
            }

            std::cout << std::dec;

            break;
        }
        case LogType::START_CHKP: {
            auto n = read_uint32(log_file);
            std::cout << "START CHKP," << n;

            for (size_t i = 0; i < n; i++) {
                auto tid = read_uint32(log_file);
                std::cout << "," << tid;
            }
            break;
        }
        case LogType::END_CHKP: {
            std::cout << "END CHKP";
            break;
        }
        }
        std::cout << ">\n";
        log_file.read(buffer, 1);
    }

    delete[] buffer;
}
