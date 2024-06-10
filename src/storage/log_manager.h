#pragma once

#include <fstream>
#include <mutex>

#include "storage/file_id.h"
#include "storage/page.h"

using TxId = uint32_t;

enum class LogType {
    START      = 0,
    COMMIT     = 1,
    ABORT      = 2,
    END        = 3,
    WRITE_UR   = 4, // undo-redo
    WRITE_U    = 5, // undo
    START_CHKP = 6,
    END_CHKP   = 7,
};

class LogManager {
public:
    LogManager();

    ~LogManager();

    void start(TxId tid);

    void commit(TxId tid);

    void abort(TxId tid);

    void end(TxId tid);

    void write_UR(TxId     tid,
                  uint32_t table_id,
                  uint32_t page_num,
                  uint32_t offset,
                  uint32_t len,
                  char*    old_bytes,
                  char*    new_bytes);

    void write_U(TxId     tid,
                 uint32_t table_id,
                 uint32_t page_num,
                 uint32_t offset,
                 uint32_t len,
                 char*    old_bytes);

    void start_chkp(const std::vector<TxId>& tids);

    void end_chkp();

    TxId get_new_tid();

private:
    std::mutex log_mutex;

    std::fstream log_file;

    char* buffer;

    TxId tx_counter = 0;
};
