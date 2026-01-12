#include <cmath>

#include <glad/glad.h>

#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/scenes/mission/menu/ConstructionMenu.hpp"


#define CORNER_SEGMENTS 8
static constexpr size_t total_vertices = 8 + 4 * CORNER_SEGMENTS;
static std::vector<float> create_rect_with_rounded_edges(float x, float y, float w, float h, float r) noexcept;


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

//  Test rectangle
    auto vertices = create_rect_with_rounded_edges(0.f, 0.f, 800.f, 600.f, 10.f);

    glCreateBuffers(1, &m_vbo);
    glNamedBufferData(m_vbo, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
    VertexArrayObject::createVertexInputState(m_vao, m_vbo, attributes);

    const float thickness = 5.f;

    m_outline.create(total_vertices, [&vertices](size_t index) -> vec2s
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


void ConstructionMenu::draw() noexcept
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, total_vertices);
    glBindVertexArray(0);
}


void ConstructionMenu::drawOutline() noexcept
{
    m_outline.draw();
}


bool ConstructionMenu::isEnabled() const noexcept
{
    return m_isEnabled;
}


const Transform2D& ConstructionMenu::getTransform() const noexcept
{
    return m_transform;
}


std::vector<float> create_rect_with_rounded_edges(float x, float y, float w, float h, float r) noexcept
{
    std::vector<float> vertices(total_vertices << 1);
    size_t vertex = 0;

    constexpr float angleStep = M_PI_2 / (float)(CORNER_SEGMENTS - 1);

    vertices[vertex++] = x + r;     vertices[vertex++] = y;
    vertices[vertex++] = x + w - r; vertices[vertex++] = y;

    float cx = x + w - r;
    float cy = y + r;
    for(int32_t i = 0; i < CORNER_SEGMENTS; ++i)
    {
        const float angle = M_PI * 3.f / 2.f + angleStep * i;
        vertices[vertex++] = cx + cos(angle) * r;
        vertices[vertex++] = cy + sin(angle) * r;
    }

    vertices[vertex++] = x + w; vertices[vertex++] = y + r;
    vertices[vertex++] = x + w; vertices[vertex++] = y + h - r;

    cx = x + w - r;
    cy = y + h - r;
    for(int32_t i = 0; i < CORNER_SEGMENTS; ++i)
    {
        const float angle = angleStep * i;
        vertices[vertex++] = cx + cos(angle) * r;
        vertices[vertex++] = cy + sin(angle) * r;
    }

    vertices[vertex++] = x + w - r; vertices[vertex++] = y + h;
    vertices[vertex++] = x + r;     vertices[vertex++] = y + h;

    cx = x + r;
    cy = y + h - r;
    for(int32_t i = 0; i < CORNER_SEGMENTS; ++i)
    {
        const float angle = M_PI_2 + angleStep * i;
        vertices[vertex++] = cx + cos(angle) * r;
        vertices[vertex++] = cy + sin(angle) * r;
    }

    vertices[vertex++] = x; vertices[vertex++] = y + h - r;
    vertices[vertex++] = x; vertices[vertex++] = y + r;

    cx = x + r;
    cy = y + r;
    for(int32_t i = 0; i < CORNER_SEGMENTS; ++i)
    {
        const float angle = M_PI + angleStep * i;
        vertices[vertex++] = cx + cos(angle) * r;
        vertices[vertex++] = cy + sin(angle) * r;
    }

    return vertices;
}