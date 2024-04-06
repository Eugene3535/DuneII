#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <string>
#include <filesystem>

struct FileProvider
{
    std::filesystem::path find_path_to_file(const std::string& name) noexcept;
};

#endif