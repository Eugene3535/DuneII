#include <array>
#include <vector>

#include <glad/glad.h>
#include <cglm/call/vec2.h>
#include <cglm/struct/vec4.h>

#include "resources/ogl/vao/VertexArrayObject.hpp"
#include "graphics/outline/Outline.hpp"


Outline::Outline(uint32_t vboHandle, uint32_t vaoHandle) noexcept:
    m_vbo(vboHandle, GL_ARRAY_BUFFER),
    m_vao(vaoHandle)
{
	m_vbo.create(sizeof(float), 0, nullptr, GL_STATIC_DRAW);

    VertexArrayObject vao(vaoHandle);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
    vao.addVertexBuffer(m_vbo, attributes);
}


void Outline::create(size_t pointCount, const std::function<vec2s(size_t)>& getPoint, float thickness) noexcept
{
    if (pointCount < 3)
        return;
    
    if(thickness < 1.f)
        return;
    
    std::vector<vec2s> vertices(pointCount + 2); // + 2 for center and repeated first point

    for (size_t i = 0; i < pointCount; ++i)
        vertices[i + 1] = getPoint(i);

    vertices[pointCount + 1] = vertices[1];
    vertices[0] = vertices[1];

    float left   = vertices[0].x;
    float top    = vertices[0].y;
    float right  = vertices[0].x;
    float bottom = vertices[0].y;

    for (const auto& vertex : vertices)
    {
        vec2s position = vertex;

        // Update left and right
        if (position.x < left)
            left = position.x;
        else if (position.x > right)
            right = position.x;

        // Update top and bottom
        if (position.y < top)
            top = position.y;
        else if (position.y > bottom)
            bottom = position.y;
    }

    vec4s bounds = { left, top, right - left, bottom - top };
    vec2s center = { bounds.x + bounds.z * 0.5f, bounds.y + bounds.w * 0.5f };
    vertices[0] = center;

    update(vertices, thickness);
}


void Outline::draw() noexcept
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vbo.getCount());
    glBindVertexArray(0);
}


void Outline::update(std::span<vec2s> points, float thickness) noexcept
{
    auto compute_normal = [](vec2 p1, vec2 p2) -> vec2s // Compute the normal of a segment
    {
        vec2 v;
        glmc_vec2_sub(p2, p1, v);
        vec2s normal = { -v[1], v[0] }; // make perpendicular
        float length = sqrt(normal.x * normal.x + normal.y * normal.y);

        if (length != 0.f)
        {
            normal.x /= length;
            normal.y /= length;
        }

        return normal;
    };

    const size_t count = points.size() - 2;
    std::vector<vec2s> vertices((count + 1) * 2);

    for (size_t i = 0; i < count; ++i)
    {
        const size_t index = i + 1;

//  Get the two segments shared by the current point
        vec2 p0, p1, p2;
        size_t j = (i == 0) ? count : index - 1;

        glmc_vec2_copy(points[j].raw, p0);
        glmc_vec2_copy(points[index].raw, p1);
        glmc_vec2_copy(points[index + 1].raw, p2);

//  Compute their normal
        vec2s n1 = compute_normal(p0, p1);
        vec2s n2 = compute_normal(p1, p2);

//  Make sure that the normals point towards the outside of the shape
//  (this depends on the order in which the points were defined)
        {
            vec2 vertex;
            glmc_vec2_sub(points[0].raw, p1, vertex);

            if(glmc_vec2_dot(n1.raw, vertex) > 0.f)
            {
                n1.x = -n1.x;
                n1.y = -n1.y;
            }

            if(glmc_vec2_dot(n2.raw, vertex) > 0.f)
            {
                n2.x = -n2.x;
                n2.y = -n2.y;
            }
        }

//  Combine them to get the extrusion direction
        const float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        vec2s normal;
        glmc_vec2_add(n1.raw, n2.raw, normal.raw);
        normal.x /= factor;
        normal.y /= factor;

//  Update the outline points
        glmc_vec2_copy(p1, vertices[i * 2 + 0].raw);
        {
            vec2 v;
            glmc_vec2_scale(normal.raw, thickness, v);
            glmc_vec2_add(p1, v, vertices[i * 2 + 1].raw);
        }
    }

//  Duplicate the first point at the end, to close the outline
    vertices[count * 2 + 0] = vertices[0];
    vertices[count * 2 + 1] = vertices[1];

//  Push to GPU
    m_vbo.update(0, sizeof(vertices[0]), vertices.size(), static_cast<const void*>(vertices.data()));
}