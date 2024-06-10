#include "system.h"

#include <stdexcept>

// memory for the global objects
static typename std::aligned_storage<sizeof(BufferManager), alignof(BufferManager)>::type buffer_mgr_buf;
static typename std::aligned_storage<sizeof(FileManager), alignof(FileManager)>::type     file_mgr_buf;
static typename std::aligned_storage<sizeof(LogManager), alignof(LogManager)>::type       log_mgr_buf;
static typename std::aligned_storage<sizeof(Catalog), alignof(Catalog)>::type             catalog_buf;

BufferManager& buffer_mgr  = reinterpret_cast<BufferManager&>(buffer_mgr_buf);
FileManager& file_mgr      = reinterpret_cast<FileManager&>(file_mgr_buf);
LogManager& log_mgr        = reinterpret_cast<LogManager&>(log_mgr_buf);
Catalog& catalog           = reinterpret_cast<Catalog&>(catalog_buf);


System::System(const std::string& db_folder, uint64_t buffer_size) {
    new (&file_mgr) FileManager(db_folder);
    new (&buffer_mgr) BufferManager(buffer_size);
    new (&log_mgr) LogManager();
    new (&catalog) Catalog("catalog.dat");
}


System System::init(const std::string& db_folder, uint64_t buffer_size) {
    if (initialized) {
        throw std::logic_error("System::init cannot be called twice");
    }
    initialized = true;
    return System(db_folder, buffer_size);
}


System::~System() {
    catalog.~Catalog();
    log_mgr.~LogManager();
    buffer_mgr.~BufferManager();
    file_mgr.~FileManager();
}
