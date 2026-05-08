#include <cglm/struct/ivec4.h>

#include "resources/gl_interfaces/texture/Texture2D.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/Engine.hpp"
#include "game/scenes/mission/HUD/previews/EntityPreview.hpp"


EntityPreview::EntityPreview(Engine* engine) noexcept:
    m_engine(engine),
    m_texture(0),
    m_defaultProgram(0),
    m_progressProgram(0),
    m_vertexBufferObject(0),
    m_vertexArrayObject(0)
{
    m_uniforms.top = 0;
    m_uniforms.bottom = 0;
    m_uniforms.progress = 0;
}


EntityPreview::~EntityPreview()
{
    glDeleteTextures(1, &m_texture);
    glDeleteVertexArrays(1, &m_vertexArrayObject);
    glDeleteBuffers(1, &m_vertexBufferObject);
}


bool EntityPreview::loadFromTexture(const Texture2D& texture) noexcept
{
//  Shader program
    m_defaultProgram = m_engine->getShaderProgram("sprite");
    m_progressProgram = m_engine->getShaderProgram("entity_view");

    if ( ! (m_defaultProgram && m_progressProgram) )
        return false;

    {// construction mode progress animation
        int32_t uniform = glGetUniformLocation(m_progressProgram, "top");
        if (uniform == -1) return false;
        m_uniforms.top = uniform;

        uniform = glGetUniformLocation(m_progressProgram, "bottom");
        if (uniform == -1) return false;
        m_uniforms.bottom = uniform;

        uniform = glGetUniformLocation(m_progressProgram, "progress");
        if (uniform == -1) return false;
        m_uniforms.progress = uniform;
    }

//  Texture
    m_texture = texture.handle;
    const int32_t columns       = 6; // The number of tiles in the texture horizontally
    const int32_t rows          = 7;    // and vertically
    const int32_t previewWidth  = texture.width / columns;
    const int32_t previewHeight = texture.height / rows;
    const vec2s   ratio         = { 1.f / texture.width, 1.f / texture.height };

    m_textureGrid.reserve((rows * columns) << 2);

    for (int32_t y = 0; y < rows; ++y)
    {
        for (int32_t x = 0; x < columns; ++x)
        {
            const ivec4s frame = { x * previewWidth, y * previewHeight, previewWidth, previewHeight };

            const float left   = frame.x * ratio.x;
            const float top    = frame.y * ratio.y;
            const float right  = (frame.x + frame.z) * ratio.x;
            const float bottom = (frame.y + frame.w) * ratio.y;

            m_textureGrid.push_back({ left,  top    });
            m_textureGrid.push_back({ right, top    });
            m_textureGrid.push_back({ right, bottom });
            m_textureGrid.push_back({ left,  bottom });
        }
    }

    return true;
}


void EntityPreview::createIcons(const ivec2s position, const ivec2s size) noexcept
{
    auto texCoords = getTexCoords(EntityIcon::Empty_Cell);

    const float x = static_cast<float>(position.x);
    const float y = static_cast<float>(position.y);
    const float w = static_cast<float>(size.x);
    const float h = static_cast<float>(size.y);

    const float offset = h + 20;

    const std::array<float, 32> vertices = 
    {
        x,     y,     texCoords[0].x, texCoords[0].y,
        x + w, y,     texCoords[1].x, texCoords[1].y,
        x + w, y + h, texCoords[2].x, texCoords[2].y,
        x,     y + h, texCoords[3].x, texCoords[3].y,

        x,     y + offset,     texCoords[0].x, texCoords[0].y,
        x + w, y + offset,     texCoords[1].x, texCoords[1].y,
        x + w, y + offset + h, texCoords[2].x, texCoords[2].y,
        x,     y + offset + h, texCoords[3].x, texCoords[3].y
    };

    glCreateBuffers(1, &m_vertexBufferObject);
    glNamedBufferData(m_vertexBufferObject, std::span<const float>(vertices).size_bytes(), vertices.data(), GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_vertexArrayObject);
    const std::array<VertexBufferLayout::Attribute, 1> attributes { VertexBufferLayout::Attribute::Float4 };
    VertexArrayObject::createVertexInputState(m_vertexArrayObject, m_vertexBufferObject, attributes);
}


void EntityPreview::setPreviewIcon(EntityIcon icon) noexcept
{
    if(icon > EntityIcon::Empty_Cell)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

    if (void* buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
    {
        auto vertices = static_cast<float*>(buffer);
        auto texCoords = getTexCoords(icon);
        
        vertices[2] = texCoords[0].x;
        vertices[3] = texCoords[0].y;

        vertices[6] = texCoords[1].x;
        vertices[7] = texCoords[1].y;

        vertices[10] = texCoords[2].x;
        vertices[11] = texCoords[2].y;

        vertices[14] = texCoords[3].x;
        vertices[15] = texCoords[3].y;

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void EntityPreview::setConstructionIcon(EntityIcon icon) noexcept
{
    if(icon > EntityIcon::Empty_Cell)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

    if (void* buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
    {
        auto vertices = static_cast<float*>(buffer) + 16;
        auto texCoords = getTexCoords(icon);
        
        vertices[2] = texCoords[0].x;
        vertices[3] = texCoords[0].y;

        vertices[6] = texCoords[1].x;
        vertices[7] = texCoords[1].y;

        vertices[10] = texCoords[2].x;
        vertices[11] = texCoords[2].y;

        vertices[14] = texCoords[3].x;
        vertices[15] = texCoords[3].y;

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void EntityPreview::draw() const noexcept
{
    glUseProgram(m_defaultProgram);
    glBindTextureUnit(0, m_texture);
    glBindVertexArray(m_vertexArrayObject);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindTextureUnit(0, 0);
}


void EntityPreview::draw(EntityIcon icon, float progress) const noexcept
{
    auto texCoords = getTexCoords(icon);

    glUseProgram(m_progressProgram);
    glUniform1f(m_uniforms.top, texCoords[0].y);
    glUniform1f(m_uniforms.bottom, texCoords[3].y);
    glUniform1f(m_uniforms.progress, progress);

    glBindTextureUnit(0, m_texture);
    glBindVertexArray(m_vertexArrayObject);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glBindTextureUnit(0, 0);
}


const vec2s* EntityPreview::getTexCoords(EntityIcon icon) const noexcept
{
    const size_t index = static_cast<size_t>(icon) << 2;

    return m_textureGrid.data() + index;
}