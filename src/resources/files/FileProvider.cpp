#include "resources/files/FileProvider.hpp"


std::filesystem::path FileProvider::findPathToFile(const std::string& filename) noexcept
{
    if(auto it = m_cachedPaths.find(filename); it != m_cachedPaths.end())
        return it->second;

    std::filesystem::path resFolder = std::filesystem::current_path() / "res";

    if(std::filesystem::exists(resFolder))
    {
        for (const auto& file : std::filesystem::recursive_directory_iterator(resFolder))
            if (file.path().filename() == filename)
            {
                auto result = m_cachedPaths.emplace(filename, file.path());

                if(result.second)
                    return result.first->second;
            } 
                
    }

    return {};
}