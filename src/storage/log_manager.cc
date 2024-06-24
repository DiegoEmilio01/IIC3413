#include "log_manager.h"

#include "relational_model/system.h"

LogManager::LogManager() {
    // buffer size must be big enough to handle a write of an entire page
    buffer = new char[Page::SIZE];

    // log disabled for now
    // auto log_path = file_mgr.get_file_path("IIC3413log.cr");
    // log_file.open(log_path, std::ios::out|std::ios::app);
}

LogManager::~LogManager() {
    delete[] buffer;
}

TxId LogManager::get_new_tid() {
    std::lock_guard<std::mutex> lock(log_mutex);
    auto res = tx_counter++;
    return res;
}


void write_int32(int32_t i, char* out) {
    char* i_ptr = reinterpret_cast<char*>(&i);

    out[0] = *(i_ptr++);
    out[1] = *(i_ptr++);
    out[2] = *(i_ptr++);
    out[3] = *(i_ptr++);
}


void LogManager::start(TxId tid) {
    std::lock_guard<std::mutex> lock(log_mutex);

    buffer[0] = static_cast<unsigned char>(LogType::START);
    write_int32(tid, &buffer[1]);

    log_file.write(buffer, 5);
}


void LogManager::commit(TxId tid) {
    std::lock_guard<std::mutex> lock(log_mutex);

    buffer[0] = static_cast<unsigned char>(LogType::COMMIT);
    write_int32(tid, &buffer[1]);

    log_file.write(buffer, 5);
}


void LogManager::abort(TxId tid) {
    std::lock_guard<std::mutex> lock(log_mutex);

    buffer[0] = static_cast<unsigned char>(LogType::ABORT);
    write_int32(tid, &buffer[1]);

    log_file.write(buffer, 5);
}


void LogManager::end(TxId tid) {
    std::lock_guard<std::mutex> lock(log_mutex);

    buffer[0] = static_cast<unsigned char>(LogType::END);
    write_int32(tid, &buffer[1]);

    log_file.write(buffer, 5);
}


void LogManager::write_UR(TxId     tid,
                          uint32_t table_id,
                          uint32_t page_num,
                          uint32_t offset,
                          uint32_t len,
                          char*    old_bytes,
                          char*    new_bytes)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    buffer[0] = static_cast<unsigned char>(LogType::WRITE_UR);
    size_t total_written = 1;
    char* current_write_ptr = &buffer[1];

    write_int32(tid, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    write_int32(table_id, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    write_int32(page_num, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    write_int32(offset, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    write_int32(len, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    memcpy(current_write_ptr, old_bytes, len);
    current_write_ptr += len;
    total_written += len;

    memcpy(current_write_ptr, new_bytes, len);
    current_write_ptr += len;
    total_written += len;

    log_file.write(buffer, total_written);
}


void LogManager::write_U(TxId     tid,
                         uint32_t table_id,
                         uint32_t page_num,
                         uint32_t offset,
                         uint32_t len,
                         char*    old_bytes) {
    std::lock_guard<std::mutex> lock(log_mutex);

    buffer[0] = static_cast<unsigned char>(LogType::WRITE_U);
    size_t total_written = 1;
    char* current_write_ptr = &buffer[1];

    write_int32(tid, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    write_int32(table_id, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    write_int32(page_num, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    write_int32(offset, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    write_int32(len, current_write_ptr);
    current_write_ptr += 4;
    total_written += 4;

    memcpy(current_write_ptr, old_bytes, len);
    current_write_ptr += len;
    total_written += len;

    log_file.write(buffer, total_written);
}


void LogManager::start_chkp(const std::vector<TxId>& tids) {
    std::lock_guard<std::mutex> lock(log_mutex);

    buffer[0] = static_cast<unsigned char>(LogType::START_CHKP);
    write_int32(tids.size(), &buffer[1]);

    for (size_t i = 0; i < tids.size(); i++) {
        write_int32(tids[i], &buffer[5 + (i*4)]);
    }
    log_file.write(buffer, 5 + (4*tids.size()));
}


void LogManager::end_chkp() {
    std::lock_guard<std::mutex> lock(log_mutex);

    buffer[0] = static_cast<unsigned char>(LogType::END_CHKP);
    log_file.write(buffer, 1);
}
