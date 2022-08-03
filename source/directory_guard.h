#pragma once

#include <filesystem>

class DirectoryGuard {
public:
    explicit DirectoryGuard(const std::filesystem::path& path);
    std::filesystem::path directory() const;
    ~DirectoryGuard();
private:
    std::filesystem::path _directory_path;
};