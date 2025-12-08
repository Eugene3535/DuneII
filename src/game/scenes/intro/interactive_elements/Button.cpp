#include <cassert>

#include <glad/glad.h>
#include <cglm/call/vec2.h>
#include <cglm/call/aabb2d.h>

#include "game/scenes/intro/interactive_elements/Button.hpp"


static constexpr float normal_color[]       = { 150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 150.f / 255.f };
static constexpr float under_cursor_color[] = { 200.f / 255.f, 200.f / 255.f, 200.f / 255.f, 200.f / 255.f };
static constexpr float is_clicked_color[]   = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 255.f / 255.f };


Button::Button(const Sprite& sprite, const int32_t uniformLocation) noexcept:
    Transform2D(),
    m_sprite(sprite),
    m_uniform(uniformLocation),
    m_bounds(),
    m_currentColor(normal_color),
    m_boundsNeedUpdate(true),
    m_isSelected(false)
{
    setOrigin(sprite.width * 0.5f, sprite.height * 0.5f);
}


void Button::update(vec2 mousePosition, bool isClicked) noexcept
{
    if(m_boundsNeedUpdate)
    {
        vec2 position, scale;  
        getPosition(position);
        getScale(scale);
        const vec2 size = { static_cast<float>(m_sprite.width) * scale[0], static_cast<float>(m_sprite.height) * scale[1] };

        position[0] -= size[0] * 0.5f;
        position[1] -= size[1] * 0.5f;
        glmc_vec2_copy(position, m_bounds[0]);

        position[0] += size[0];
        position[1] += size[1];
        glmc_vec2_copy(position, m_bounds[1]);

        m_boundsNeedUpdate = false;
    }

    m_isSelected = false;

    m_currentColor = normal_color;
    const bool isUnderCursor = glmc_aabb2d_point(m_bounds, mousePosition);

    if(isUnderCursor)
        m_currentColor = isClicked ? is_clicked_color : under_cursor_color;

    if(isClicked && isUnderCursor)
        m_isSelected = true;
}


void Button::draw() noexcept
{
    assert(m_uniform != -1);

    glUniform4fv(m_uniform, 1, m_currentColor);
    glBindTexture(GL_TEXTURE_2D, m_sprite.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_sprite.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Button::resize(int width, int height) noexcept
{
    assert(m_sprite.width > 0);
    assert(m_sprite.height > 0);

    float dx = static_cast<float>(width) / m_sprite.width;
    float dy = static_cast<float>(height) / m_sprite.height;
    setScale(dx, dy);

    m_boundsNeedUpdate = true;
}


bool Button::isSelected() const noexcept
{
    return m_isSelected;
}