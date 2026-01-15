#include <cmath>

#include <glad/glad.h>

#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "graphics/geometry/GeometryGenerator.hpp"
#include "game/scenes/mission/menu/ConstructionMenu.hpp"


ConstructionMenu::ConstructionMenu(const ivec2s& windowSize) noexcept:
    m_windowSize(windowSize),
    m_transform(),
    m_vao(0),
    m_vbo(0),
    m_count(0),
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

//  Test rectangle
    auto vertices = generator.createRectWithRoundedEdges(0.f, 0.f, 800.f, 600.f, 10.f);
    m_count = (vertices.size() >> 1);

    glCreateBuffers(1, &m_vbo);
    glNamedBufferData(m_vbo, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
    VertexArrayObject::createVertexInputState(m_vao, m_vbo, attributes);

    const float thickness = 5.f;

    m_outline.create(m_count, [&vertices](size_t index) -> vec2s
    {
        const float* data = vertices.data() + (index << 1);

        return { data[0], data[1] };
    }, thickness);
}


void ConstructionMenu::update() noexcept
{
    if(m_isEnabled)
    {
        vec2s center = { m_windowSize.x * 0.5f, m_windowSize.y * 0.5f };

        m_transform.setOrigin(400, 300);
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
        {
            const float outlineColor[] = { 155.f / 255.f, 160.f / 255.f, 163.f / 255.f, 1.f };
            glUniform4fv(uniformColor, 1, outlineColor);
            glBindVertexArray(m_vao);
            glDrawArrays(GL_TRIANGLE_FAN, 0, m_count);
            glBindVertexArray(0);
        }
        {
            const float outlineColor[] = { 170.f / 255.f, 199.f / 255.f, 207.f / 255.f, 1.f };
            glUniform4fv(uniformColor, 1, outlineColor);
            m_outline.draw();
        }  
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