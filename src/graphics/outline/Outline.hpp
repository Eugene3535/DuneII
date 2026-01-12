#ifndef OUTLINE_HPP
#define OUTLINE_HPP

#include <span>
#include <functional>

#include <cglm/struct/vec2.h>


class Outline
{
public:
    Outline()                            noexcept;
    Outline(const Outline&)              noexcept = delete;
    Outline(Outline&&)                   noexcept = delete;
    Outline& operator = (const Outline&) noexcept = delete;
    Outline& operator = (Outline&&)      noexcept = delete;
    ~Outline();

    void create(size_t pointCount, const std::function<vec2s(size_t)>& getPoint, float thickness = 1.f) noexcept;
    void draw() noexcept;

private:
    void update(std::span<vec2s> points, float thickness) noexcept;

    uint32_t m_vbo;
    uint32_t m_vao;
    uint32_t m_count;
};

#endif // !OUTLINE_HPP