#include <glad/glad.h>

#include "files/StbImage.hpp"
#include "graphics/texture/Texture2D.hpp"


Texture2D::Texture2D(uint32_t handle) noexcept:
    m_handle(handle),
    m_isSmooth(false),
	m_isRepeated(false)
{

}


Texture2D::~Texture2D() = default;


bool Texture2D::loadFromImage(const StbImage& image) noexcept
{
    if(m_handle)
    {
        int width  = image.width;
        int height = image.height;

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

        glTextureStorage2D(m_handle, 1, internalFormat, width, height);
        glTextureSubImage2D(m_handle, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, image.pixels.get());
        glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
    if(m_handle)
    {
        if(m_isSmooth != smooth)
        {
            m_isSmooth = smooth;
            glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
            glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
        }
    }
}


void Texture2D::setRepeated(bool repeate) noexcept
{
    if(m_handle)
    {
        if(m_isRepeated != repeate)
        {
            m_isRepeated = repeate;

            if (m_isRepeated)
            {
                glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else
            {
                glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

                constexpr static float border_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
                glTextureParameterfv(m_handle, GL_TEXTURE_BORDER_COLOR, border_color);
            }
        }
    }
}


ivec2s Texture2D::getSize() const noexcept
{
    if (m_handle)
    {
        GLint width;
        GLint height;
        glGetTextureLevelParameteriv(m_handle, 0, GL_TEXTURE_WIDTH, &width);
        glGetTextureLevelParameteriv(m_handle, 0, GL_TEXTURE_HEIGHT, &height);

        return { width, height };
    }

    return { 0, 0 };
}


uint32_t Texture2D::getHandle() const noexcept
{
    return m_handle;
}