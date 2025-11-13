#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "graphics/Meshes.hpp"
#include "graphics/Transform2D.hpp"

class Button
{
public:
    Button(const Sprite& sprite) noexcept;

    void setPosition(const glm::vec2& position) noexcept;
    void resize(const glm::vec2& newSize) noexcept;

    void update(const glm::ivec2& mousePosition, bool isClicked) noexcept;
    void draw() noexcept;

    const Transform2D& getTransform() const noexcept; 
    bool isPressed() const noexcept;

private:
    const Sprite m_sprite;
    Transform2D  m_transform;
    glm::vec4    m_bounds;
    const float* m_currentColor;
    bool         m_boundsNeedUpdate;
    bool         m_isPressed;
};

#endif // !BUTTON_HPP