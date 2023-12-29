#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <string>
#include <filesystem>

struct FileProvider
{
    static std::filesystem::path getPathToFile(const std::string& fName);
};

#endif