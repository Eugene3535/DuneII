#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <span>

#include "graphics/Meshes.hpp"
#include "graphics/transform/Transform2D.hpp"


class HeadUpDisplay
{
public:
    HeadUpDisplay(const class Builder& builder) noexcept;
    ~HeadUpDisplay();

    void initCrosshairs(std::span<const Sprite> crosshairs) noexcept;

    void update(const vec2s position, float dt) noexcept;
    void draw() const noexcept;

    const Transform2D& getTransform() const noexcept;

private:
    const class Builder& m_builder;

    Sprite m_cursor;
    Transform2D m_transform;
};

#endif // !HEAD_UP_DISPLAY_HPP