#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "graphics/Meshes.hpp"
#include "graphics/transform/Transform2D.hpp"

class Button:
    public Transform2D
{
public:
    Button(const mesh::Sprite& sprite, const int32_t uniformLocation) noexcept;

    void update(vec2s mousePosition, bool isClicked) noexcept;
    void draw() noexcept;
    void resize(int width, int height) noexcept;
    bool isSelected() const noexcept;

private:
    const mesh::Sprite m_sprite;
    const int32_t      m_uniform;

    vec2 m_bounds[2];
    const float* m_currentColor;

    bool m_boundsNeedUpdate;
    bool m_isSelected;
};

#endif // !BUTTON_HPP