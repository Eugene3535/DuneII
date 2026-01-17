#include <cmath>

#include <glad/glad.h>

#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "graphics/geometry/GeometryGenerator.hpp"
#include "game/scenes/mission/menu/ConstructionMenu.hpp"


static constexpr float background_color[]      = { 155.f / 255.f, 160.f / 255.f, 163.f / 255.f, 1.f };
static constexpr float outline_color[]         = { 170.f / 255.f, 199.f / 255.f, 207.f / 255.f, 1.f };
static constexpr float cell_background_color[] = { 116.f / 255.f, 120.f / 255.f, 121.f / 255.f, 1.f };


ConstructionMenu::ConstructionMenu(const ivec2s& windowSize) noexcept:
    m_windowSize(windowSize),
    m_transform(),
    m_vao(0),
    m_vbo(0),
    m_isEnabled(false)
{

}


ConstructionMenu::~ConstructionMenu()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}


void ConstructionMenu::init() noexcept
{
    if(m_vbo && m_vao)
        return;

    GeometryGenerator generator;
    std::vector<float> buffer;
    const float thickness = 5.f;

    {// Background
        auto rectVertices = generator.createRectWithRoundedEdges(0.f, 0.f, 920.f, 800.f, 10.f);
        m_root.background = (rectVertices.size() >> 1);

        auto outlineVertices = generator.createOutline(m_root.background, [&rectVertices](size_t index) -> vec2s
        {
            const float* data = rectVertices.data() + (index << 1);

            return { data[0], data[1] };
        }, thickness);
        m_root.outline = (outlineVertices.size() >> 1);

        buffer.insert(buffer.end(), rectVertices.begin(), rectVertices.end());
        buffer.insert(buffer.end(), outlineVertices.begin(), outlineVertices.end());
    }

    {// Building
        auto rectVertices = generator.createRectWithRoundedEdges(580.f, 50.f, 300.f, 150.f, 10.f);
        m_building.background = (rectVertices.size() >> 1);

        auto outlineVertices = generator.createOutline(m_building.background, [&rectVertices](size_t index) -> vec2s
        {
            const float* data = rectVertices.data() + (index << 1);

            return { data[0], data[1] };
        }, thickness);
        m_building.outline = (outlineVertices.size() >> 1);

        buffer.insert(buffer.end(), rectVertices.begin(), rectVertices.end());
        buffer.insert(buffer.end(), outlineVertices.begin(), outlineVertices.end());
    }

//  Unload to GPU
    glCreateBuffers(1, &m_vbo);
    glNamedBufferData(m_vbo, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
    VertexArrayObject::createVertexInputState(m_vao, m_vbo, attributes);
}


void ConstructionMenu::update() noexcept
{
    if(m_isEnabled)
    {
        const vec2s center = { m_windowSize.x * 0.5f, m_windowSize.y * 0.5f };

        m_transform.setOrigin(460, 400);
        m_transform.setPosition(center);
    }
}


void ConstructionMenu::enable() noexcept
{
    m_isEnabled = true;
}


void ConstructionMenu::disable() noexcept
{
    m_isEnabled = false;
}


void ConstructionMenu::draw(uint32_t program) noexcept
{
    glUseProgram(program);

    if(GLint uniformColor = glGetUniformLocation(program, "outlineColor"); uniformColor != -1)
    {
        uint32_t startFrame = 0;

        glBindVertexArray(m_vao);

        glUniform4fv(uniformColor, 1, background_color);
        glDrawArrays(GL_TRIANGLE_FAN, 0, m_root.background);
        glUniform4fv(uniformColor, 1, outline_color);
        glDrawArrays(GL_TRIANGLE_STRIP, m_root.background, m_root.outline);
        startFrame = m_root.background + m_root.outline;

        glUniform4fv(uniformColor, 1, cell_background_color);
        glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_building.background);
        glUniform4fv(uniformColor, 1, outline_color);
        glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_building.background, m_building.outline);

        glBindVertexArray(0);
    }
}


bool ConstructionMenu::isEnabled() const noexcept
{
    return m_isEnabled;
}


const Transform2D& ConstructionMenu::getTransform() const noexcept
{
    return m_transform;
}