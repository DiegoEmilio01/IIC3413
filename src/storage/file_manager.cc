#include "file_manager.h"

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>

#include "storage/filesystem.h"

using namespace std;

FileManager::FileManager(const std::string& db_folder) :
    db_folder(db_folder)
{
    if (Filesystem::exists(db_folder)) {
        if (!Filesystem::is_directory(db_folder)) {
            throw std::invalid_argument("Cannot create database directory: \""
                                        + db_folder
                                        + "\", a file with that name already exists.");
        }
    } else {
        Filesystem::create_directories(db_folder);
    }
}


void FileManager::flush(Page& page) const {
    auto fd = page.page_id.file_id.id;
    lseek(fd, page.page_id.page_number*Page::SIZE, SEEK_SET);
    auto write_res = write(fd, page.data(), Page::SIZE);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into file when flushing page");
    }
    page.dirty = false;
}


void FileManager::read_page(PageId page_id, char* bytes) const {
    auto fd = page_id.file_id.id;
    lseek(fd, 0, SEEK_END);

    struct stat buf;
    fstat(fd, &buf);
    uint64_t file_size = buf.st_size;

    lseek(fd, page_id.page_number*Page::SIZE, SEEK_SET);
    if (file_size/Page::SIZE <= page_id.page_number) {
        // new file page, write zeros
        memset(bytes, 0, Page::SIZE);
        auto write_res = ftruncate(fd, Page::SIZE * (page_id.page_number + 1));

        if (write_res == -1) {
            throw std::runtime_error("Could not write into file");
        }
    } else {
        // reading existing file page
        auto read_res = read(fd, bytes, Page::SIZE);
        if (read_res == -1) {
            throw std::runtime_error("Could not read file page");
        }
    }
}


FileId FileManager::get_file_id(const string& filename) {
    auto search = filename2file_id.find(filename);
    if (search != filename2file_id.end()) {
        return search->second;
    } else {
        const auto file_path = get_file_path(filename);

        auto fd = open(file_path.c_str(), O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
        if (fd == -1) {
            throw std::runtime_error("Could not open file " + file_path);
        }
        const auto res = FileId(fd);
        filename2file_id.insert({ filename, res });
        return res;
    }
}
