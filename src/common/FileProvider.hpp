#ifndef FILE_PROVIDER_HPP
#define FILE_PROVIDER_HPP

#include <string>
#include <filesystem>

struct FileProvider
{
    std::filesystem::path findPathToFile(const std::string& name) noexcept;
};

#endif // !FILE_PROVIDER_HPP