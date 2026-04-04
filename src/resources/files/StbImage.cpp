#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "resources/files/StbImage.hpp"

void StbImage::StbImageDeleter::operator()(uint8_t* src) noexcept
{
    stbi_image_free(src);
}

bool StbImage::loadFromFile(const std::filesystem::path& filepath) noexcept
{
    std::string path(filepath.generic_string());

    if (stbi_uc* src = stbi_load(path.c_str(), &width, &height, &bytePerPixel, 0); src != nullptr)
    {
        pixels.reset(src);

        return true;
    }

    return false;
}