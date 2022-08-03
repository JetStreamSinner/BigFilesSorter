#include "directory_guard.h"

DirectoryGuard::DirectoryGuard(const std::filesystem::path &path) : _directory_path(path) {
    if (std::filesystem::exists(_directory_path)) {
        std::filesystem::remove_all(_directory_path);
    }
    std::filesystem::create_directory(_directory_path);
}

std::filesystem::path DirectoryGuard::directory() const {
    return _directory_path;
}

DirectoryGuard::~DirectoryGuard() {
    std::filesystem::remove_all(_directory_path);
}