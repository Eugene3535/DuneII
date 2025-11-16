#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "graphics/Meshes.hpp"
#include "graphics/Transform2D.hpp"

class Button:
    public Transform2D
{
public:
    Button(const Sprite& sprite, const int32_t uniformLocation) noexcept;

    void update(vec2 mousePosition, bool isClicked) noexcept;
    void draw() noexcept;

    void resize(int width, int height) noexcept;

private:
    const Sprite  m_sprite;
    const int32_t m_uniform;

    vec2 m_bounds[2];
    const float* m_currentColor;

    bool m_boundsNeedUpdate;
};

#endif // !BUTTON_HPP