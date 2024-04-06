#include "utils/FileProvider.hpp"

std::filesystem::path FileProvider::find_path_to_file(const std::string& name) noexcept
{
    static std::filesystem::path resource_folder;

    if (resource_folder.empty())
        resource_folder = std::filesystem::current_path() / "res";

    for (const auto& file : std::filesystem::recursive_directory_iterator(resource_folder))
        if (file.path().filename() == name)    
            return file.path();
        
    return std::filesystem::path();
}