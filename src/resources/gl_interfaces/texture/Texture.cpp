#include <cstring>

#include "resources/files/StbImage.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"


bool Texture::loadFromImage(const StbImage& image) noexcept
{
    if(handle)
    {
        width  = image.width;
        height = image.height;
        type   = GL_TEXTURE_2D; // TODO -> type move to arguments

        format = GL_RGBA;
        if(image.bytePerPixel == 1) format = GL_RED;
        if(image.bytePerPixel == 3) format = GL_RGB;

        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, format, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, format, GL_UNSIGNED_BYTE, static_cast<const void*>(image.pixels.get()));
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    return false;
}


bool Texture::loadFromFile(const std::filesystem::path& filePath) noexcept
{
    StbImage image;

    if (image.loadFromFile(filePath))
        return loadFromImage(image);
    
    return false;
}


bool Texture::loadCubeMap(const std::array<std::filesystem::path, 6>& facePaths) noexcept
{
    if(handle)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
        type = GL_TEXTURE_CUBE_MAP;

        for (size_t i = 0; i < facePaths.size(); ++i)
        {
            if(StbImage image; image.loadFromFile(facePaths[i]))
            {
                width  += image.width;
                height += image.height;

                format = GL_RGBA;
                if(image.bytePerPixel == 1) format = GL_RED;
                if(image.bytePerPixel == 3) format = GL_RGB;
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, static_cast<const void*>(image.pixels.get()));
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        return true;
    }

    return false;
}


void Texture::setSmooth(bool smooth) noexcept
{
    if(handle)
    {
        if(isSmooth != smooth)
        {
            isSmooth = smooth;

            glBindTexture(GL_TEXTURE_2D, handle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isSmooth ? GL_LINEAR : GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, isSmooth ? GL_LINEAR : GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}


void Texture::setRepeated(bool repeate) noexcept
{
    if(handle)
    {
        if(isRepeated != repeate)
        {
            isRepeated = repeate;
            glBindTexture(GL_TEXTURE_2D, handle);

            if (isRepeated)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

                constexpr static float border_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
            }

            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}