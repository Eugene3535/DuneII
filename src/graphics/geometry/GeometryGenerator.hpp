#ifndef GEOMETRY_GENERATOR_HPP
#define GEOMETRY_GENERATOR_HPP

#include <vector>
#include <span>
#include <functional>

#include <cglm/struct/vec2.h>


class GeometryGenerator
{
public:
    std::vector<float> createOutline(size_t pointCount, const std::function<vec2s(size_t)>& getPoint, float thickness = 1.f) noexcept;
    std::vector<float> createRectWithRoundedEdges(float x, float y, float w, float h, float r) noexcept;
};

#endif // !GEOMETRY_GENERATOR_HPP