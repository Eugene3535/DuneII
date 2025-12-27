#ifndef FILE_PROVIDER_HPP
#define FILE_PROVIDER_HPP

#include <string>
#include <string_view>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "common/Assets.hpp"


struct FileProvider
{
    static std::filesystem::path findPathToFile(const std::string& filename) noexcept;
};

#endif // !FILE_PROVIDER_HPP