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
    std::filesystem::path findPathToFile(const std::string& filename) noexcept;
    std::vector<std::filesystem::path> findShaders(std::string_view filename) noexcept;

private:
    std::unordered_map<std::string, std::filesystem::path> m_cachedPaths;
};

#endif // !FILE_PROVIDER_HPP