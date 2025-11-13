#ifndef FILE_PROVIDER_HPP
#define FILE_PROVIDER_HPP

#include <string>
#include <filesystem>

#include "common/Assets.hpp"


struct FileProvider
{
    std::filesystem::path findPathToFile(const std::string& filename) noexcept;
};

#endif // !FILE_PROVIDER_HPP