#define _USE_MATH_DEFINES
#include <cmath>

#include <glad/glad.h>

#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/scenes/mission/menu/ConstructionMenu.hpp"


#define CORNER_SEGMENTS 12
static std::vector<float> create_rect_with_rounded_edges(float x, float y, float w, float h, float r) noexcept;


ConstructionMenu::ConstructionMenu() noexcept:
    m_vao(0),
    m_vbo(0),
    m_isEnabled(false)
{
	glCreateBuffers(1, &m_vbo);
	glNamedBufferData(m_vbo, 0, nullptr, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
	VertexArrayObject::createVertexInputState(m_vao, m_vbo, attributes);
}


ConstructionMenu::~ConstructionMenu()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}


void ConstructionMenu::createMenu() noexcept
{
    auto vertices = create_rect_with_rounded_edges(100.f, 100.f, 800.f, 600.f, 10.f);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ConstructionMenu::showMenuForEntity() noexcept
{

}


void ConstructionMenu::draw() noexcept
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 8 + 4 * CORNER_SEGMENTS);
    glBindVertexArray(0);
}


std::vector<float> create_rect_with_rounded_edges(float x, float y, float w, float h, float r) noexcept
{
    const float minDimension = w < h ? w : h;

    if (r > minDimension * 0.5f)
        r = minDimension * 0.5f;

    constexpr size_t totalVertices = 8 + 4 * CORNER_SEGMENTS;

    std::vector<float> vertices(totalVertices);
    size_t vertex = 0;

    vertices[vertex++] = x + r;     vertices[vertex++] = y;
    vertices[vertex++] = x + w - r; vertices[vertex++] = y;
    vertices[vertex++] = x + w;     vertices[vertex++] = y + r;
    vertices[vertex++] = x + w;     vertices[vertex++] = y + h - r;
    vertices[vertex++] = x + w - r; vertices[vertex++] = y + h;
    vertices[vertex++] = x + r;     vertices[vertex++] = y + h;
    vertices[vertex++] = x;         vertices[vertex++] = y + h - r;
    vertices[vertex++] = x;         vertices[vertex++] = y + r;

    constexpr float angleStep = M_PI_2 / (float)(CORNER_SEGMENTS - 1);

    float cx = x + w - r;
    float cy = y + h - r;
    for(int32_t i = 0; i < CORNER_SEGMENTS; ++i)
    {
        const float angle = angleStep * i;
        vertices[vertex++] = cx + cos(angle) * r;
        vertices[vertex++] = cy + sin(angle) * r;
    }

    cx = x + r;
    cy = y + h - r;
    for(int32_t i = 0; i < CORNER_SEGMENTS; ++i)
    {
        const float angle = M_PI_2 + angleStep * i;
        vertices[vertex++] = cx + cos(angle) * r;
        vertices[vertex++] = cy + sin(angle) * r;
    }

    cx = x + r;
    cy = y + r;
    for(int32_t i = 0; i < CORNER_SEGMENTS; ++i)
    {
        const float angle = M_PI + angleStep * i;
        vertices[vertex++] = cx + cos(angle) * r;
        vertices[vertex++] = cy + sin(angle) * r;
    }

    cx = x + w - r;
    cy = y + r;
    for(int32_t i = 0; i < CORNER_SEGMENTS; ++i)
    {
        const float angle = M_PI * 3.f / 2.f + angleStep * i;
        vertices[vertex++] = cx + cos(angle) * r;
        vertices[vertex++] = cy + sin(angle) * r;
    }

    return vertices;
}