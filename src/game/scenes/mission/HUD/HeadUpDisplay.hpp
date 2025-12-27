#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <span>

#include "graphics/Meshes.hpp"


class HeadUpDisplay
{
public:
    HeadUpDisplay(const class Builder& builder) noexcept;
    ~HeadUpDisplay();

    void initCrosshairs(std::span<const Sprite> crosshairs) noexcept;

    void draw() const noexcept;

private:
    const class Builder& m_builder;

    Sprite m_cursor;
};

#endif // !HEAD_UP_DISPLAY_HPP