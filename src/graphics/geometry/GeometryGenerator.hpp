#pragma once

#include <vector>
#include <functional>

#include <cglm/struct/vec2.h>


class GeometryGenerator
{
public:
    template<class Callback>
    std::vector<float> createOutline(size_t pointCount, Callback&& getPoint, float thickness = 1.f) noexcept
    {
        std::vector<vec2s> vertices;

        if (pointCount < 3)
            return {};

        if (thickness < 1.f)
            return {};

        vertices.resize(pointCount + 2); // + 2 for center and repeated first point

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
        calculateOutline(vertices, result, thickness);

        return result;
    }

    std::vector<float> createRectWithRoundedEdges(float x, float y, float w, float h, float r) noexcept;

private:
    void calculateOutline(std::span<vec2s> points, std::vector<float>& result, float thickness) noexcept;
};
