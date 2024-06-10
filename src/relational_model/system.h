#pragma once

#include "relational_model/catalog.h"
#include "storage/buffer_manager.h"
#include "storage/log_manager.h"
#include "storage/file_manager.h"

class System {
public:
    ~System();

    static System init(const std::string& db_folder, uint64_t buffer_size);

private:
    static inline bool initialized = false;

    System(const std::string& db_folder, uint64_t buffer_size);
};


// global objects
extern FileManager& file_mgr;
extern BufferManager& buffer_mgr;
extern LogManager& log_mgr;
extern Catalog& catalog;
