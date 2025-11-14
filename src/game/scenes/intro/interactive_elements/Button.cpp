#include <cassert>

#include <glad/glad.h>

#include "game/scenes/intro/interactive_elements/Button.hpp"


static constexpr float button_colors[] = 
{
    150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 150.f / 255.f, // normal color
    200.f / 255.f, 200.f / 255.f, 200.f / 255.f, 200.f / 255.f, // under cursor
    255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 255.f / 255.f  // is clicked
};


static bool contains(const glm::vec2& point, const glm::vec4& rect) noexcept
{
    return (point.x > rect.x && point.x < rect.w && point.y > rect.y && point.y <= rect.z);
}



Button::Button(const Sprite& sprite) noexcept:
    Transform2D(),
    m_sprite(sprite),
    m_bounds(),
    m_currentColor(nullptr),
    m_boundsNeedUpdate(true),
    m_isPressed(false)
{
    setOrigin(glm::vec2(sprite.width / 2, sprite.height / 2));
    m_currentColor = button_colors;
}


void Button::resize(const glm::vec2& newSize) noexcept
{
    assert(m_sprite.width > 0);
    assert(m_sprite.height > 0);

    float dx = newSize.x / m_sprite.width;
    float dy = newSize.y / m_sprite.height;
    setScale(dx, dy);

    m_boundsNeedUpdate = true;
}


void Button::update(const glm::ivec2& mousePosition, bool isClicked) noexcept
{
    if(m_boundsNeedUpdate)
    {
        const glm::vec2 position = getPosition();
        const glm::vec2 scale = getScale();
        const glm::vec2 size = { static_cast<float>(m_sprite.width) * scale.x, static_cast<float>(m_sprite.height) * scale.y };

        m_bounds = { position, size };
        m_boundsNeedUpdate = false;
    }

    m_currentColor = button_colors;
    const bool isUnderCursor = contains(glm::vec2(mousePosition), m_bounds);
    m_isPressed = false;

    if(isClicked)
    {
        if(isUnderCursor)
        {
            m_currentColor = button_colors + 8; // offset to clicked button color
            m_isPressed = true;
        }
    }
    else
    {
        if(isUnderCursor)
            m_currentColor = button_colors + 4; // offset to under cursor button color
    }
}


void Button::draw() noexcept
{
    glBindTexture(GL_TEXTURE_2D, m_sprite.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_sprite.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}


bool Button::isPressed() const noexcept
{
    return m_isPressed;
}