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


std::vector<std::filesystem::path> FileProvider::findShaders(std::string_view filename) noexcept
{
    auto remove_extension = [](std::string_view filename) -> std::string 
    {
        size_t lastSlash = filename.find_last_of("/\\");

        std::string_view pureName = (lastSlash == std::string_view::npos) 
                                ? filename 
                                : filename.substr(lastSlash + 1);
        
        size_t lastDot = pureName.find_last_of('.');
        
        if (lastDot == std::string_view::npos || lastDot == 0) 
            return std::string(filename);
        
        return std::string(filename.substr(0, lastSlash == std::string_view::npos 
                                                ? lastDot 
                                                : lastSlash + 1 + lastDot));
    };

    std::filesystem::path shaderFolder = std::filesystem::current_path() / "res/shaders";
    std::vector<std::filesystem::path> shaders;

    if(std::filesystem::exists(shaderFolder))
    {
        const auto shaderName = remove_extension(filename);

        for(const auto extension : { ".vert", ".frag", ".geom" })
        {
            for (const auto& file : std::filesystem::recursive_directory_iterator(shaderFolder))
            {
                if (file.path().filename() == (shaderName + extension))
                {
                    shaders.emplace_back(file.path());
                }
            }
        }
    }

    return shaders;
}