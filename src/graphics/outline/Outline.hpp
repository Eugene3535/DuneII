#ifndef OUTLINE_HPP
#define OUTLINE_HPP

#include <span>
#include <functional>

#include <cglm/struct/vec2.h>

#include "resources/ogl/buffers/GlBuffer.hpp"


class Outline
{
public:
    Outline(uint32_t vboHandle, uint32_t vaoHandle) noexcept;

    void create(size_t pointCount, const std::function<vec2s(size_t)>& getPoint, float thickness = 1.f) noexcept;
    void draw() noexcept;

private:
    void update(std::span<vec2s> points, float thickness) noexcept;

    GLBuffer m_vbo;
    uint32_t m_vao;
};

#endif // !OUTLINE_HPP