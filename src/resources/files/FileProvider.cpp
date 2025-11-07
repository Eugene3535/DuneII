#include "resources/files/FileProvider.hpp"

std::filesystem::path FileProvider::findPathToFile(const std::string& filename) noexcept
{
    std::filesystem::path resFolder = std::filesystem::current_path() / "res";

    if(std::filesystem::exists(resFolder))
    {
        for (const auto& file : std::filesystem::recursive_directory_iterator(resFolder))
            if (file.path().filename() == filename)    
                return file.path();
    }

    return std::filesystem::path();
}