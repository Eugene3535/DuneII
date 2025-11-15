#include <cassert>

#include <glad/glad.h>

#include "game/scenes/intro/interactive_elements/Button.hpp"


static constexpr float normal_color[] = { 150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 150.f / 255.f };
static constexpr float under_cursor_color[] = { 200.f / 255.f, 200.f / 255.f, 200.f / 255.f, 200.f / 255.f };
static constexpr float is_clicked_color[] = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 255.f / 255.f };


Button::Button(const Sprite& sprite) noexcept:
    Transform2D(),
    m_sprite(sprite),
    m_bounds(),
    m_currentColor(normal_color),
    m_boundsNeedUpdate(true)
{
    setOrigin(sprite.width * 0.5f, sprite.height * 0.5f);
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

        m_bounds = { position - size * 0.5f, position + size };
        m_boundsNeedUpdate = false;
    }

    auto contains = [](const glm::vec2& point, const glm::vec4& rect) noexcept -> bool
    {
        return ((point.x > rect.x) && (point.x < rect.z) && (point.y > rect.y) && (point.y < rect.w));
    };

    m_currentColor = normal_color;
    const bool isUnderCursor = contains(glm::vec2(mousePosition), m_bounds);

    if(isUnderCursor)
    {
        if(isClicked)
            m_currentColor = is_clicked_color;      
        else
            m_currentColor = under_cursor_color;
    }
}


void Button::draw() noexcept
{
    glBindTexture(GL_TEXTURE_2D, m_sprite.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_sprite.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}


const float* Button::getColor() const noexcept
{
    return m_currentColor;
}