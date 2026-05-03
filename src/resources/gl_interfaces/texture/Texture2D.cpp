#include <glad/glad.h>

#include "resources/files/StbImage.hpp"
#include "resources/gl_interfaces/texture/Texture2D.hpp"


bool Texture2D::loadFromImage(const StbImage& image) noexcept
{
    if(handle)
    {
        width  = image.width;
        height = image.height;

        GLenum internalFormat = GL_RGBA8; 
        GLenum dataFormat = GL_RGBA;

        if (image.bytePerPixel == 3) 
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        } 
        else if (image.bytePerPixel == 1) 
        {
            internalFormat = GL_R8;
            dataFormat = GL_RED;
        }

        glTextureStorage2D(handle, 1, internalFormat, width, height);
        glTextureSubImage2D(handle, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, image.pixels.get());
        glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        return true;
    }

    return false;
}


bool Texture2D::loadFromFile(const std::filesystem::path& filePath) noexcept
{
    StbImage image;

    if (image.loadFromFile(filePath))
        return loadFromImage(image);
    
    return false;
}


void Texture2D::setSmooth(bool smooth) noexcept
{
    if(handle)
    {
        if(isSmooth != smooth)
        {
            isSmooth = smooth;
            glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, isSmooth ? GL_LINEAR : GL_NEAREST);
            glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, isSmooth ? GL_LINEAR : GL_NEAREST);
        }
    }
}


void Texture2D::setRepeated(bool repeate) noexcept
{
    if(handle)
    {
        if(isRepeated != repeate)
        {
            isRepeated = repeate;

            if (isRepeated)
            {
                glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else
            {
                glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

                constexpr static float border_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
                glTextureParameterfv(handle, GL_TEXTURE_BORDER_COLOR, border_color);
            }
        }
    }
}