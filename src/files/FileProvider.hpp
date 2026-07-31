#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <vector>

#include "common/Assets.hpp"


class FileProvider
{
public:
    FileProvider(const char* argv) noexcept;

    static std::filesystem::path findPathToFile(const std::string& filename) noexcept;
    static std::vector<std::filesystem::path> findShaders(std::string_view filename) noexcept;

private:
    std::filesystem::path m_exeDir;
};
