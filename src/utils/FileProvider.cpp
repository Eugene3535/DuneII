#include <filesystem>
#include <algorithm>

#include "utils/FileProvider.hpp"

std::string FileProvider::getPathToFile(const std::string& filename)
{
    static std::string resource_folder;

    if (resource_folder.empty())
        resource_folder = std::filesystem::current_path().string() + "/res";

    for (auto& file : std::filesystem::recursive_directory_iterator(resource_folder))
        if (file.path().filename() == filename)
        {
            std::string filepath = file.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');
            return filepath;
        }

    return std::string();
}