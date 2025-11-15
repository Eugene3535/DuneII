#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "graphics/Meshes.hpp"
#include "graphics/Transform2D.hpp"

class Button:
    public Transform2D
{
public:
    Button(const Sprite& sprite) noexcept;

    void update(vec2 mousePosition) noexcept;
    void draw() noexcept;

    void resize(int width, int height) noexcept;
    void click() noexcept;

    const float* getColor() const noexcept;

private:
    const Sprite m_sprite;
    vec2         m_bounds[2];
    const float* m_currentColor;

    bool m_boundsNeedUpdate;
    bool m_isClicked;
};

#endif // !BUTTON_HPP