#include <cglm/call/vec2.h>

#include "graphics/geometry/GeometryGenerator.hpp"


#define CORNER_SEGMENTS 8


static void calculate_outline(std::span<vec2s> points, std::vector<float>& result, float thickness) noexcept;


std::vector<float> GeometryGenerator::createOutline(size_t pointCount, const std::function<vec2s(size_t)>& getPoint, float thickness) noexcept
{
    std::vector<vec2s> vertices;

    if (pointCount < 3)
        return {};
    
    if(thickness < 1.f)
        return {};

    vertices.resize(pointCount + 2); // + 2 for center and repeated first poin

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

    std::vector<float> result;
    calculate_outline(vertices, result, thickness);

    return result;
}


std::vector<float> GeometryGenerator::createRectWithRoundedEdges(float x, float y, float w, float h, float r) noexcept
{
    static constexpr size_t totalVertices = 8 + 4 * CORNER_SEGMENTS;
    std::vector<float> vertices(totalVertices << 1);
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



void calculate_outline(std::span<vec2s> points, std::vector<float>& result, float thickness) noexcept
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

    result.resize(vertices.size() << 1);

    memcpy(result.data(), vertices.data(), vertices.size() * sizeof(vec2s));
}