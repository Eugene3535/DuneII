#include "utils/FileProvider.hpp"

std::filesystem::path FileProvider::getPathToFile(const std::string& fName)
{
    static std::filesystem::path resource_folder;

    if (resource_folder.empty())
        resource_folder = std::filesystem::current_path() / "res";

    for (auto& file : std::filesystem::recursive_directory_iterator(resource_folder))
        if (file.path().filename() == fName)    
            return file.path();
        
    return std::filesystem::path();
}