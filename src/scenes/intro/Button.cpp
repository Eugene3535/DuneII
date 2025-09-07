#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "scenes/intro/Button.hpp"


Button::Button(const sf::Texture* texture) noexcept:
    m_sprite(*texture),
    m_boundsNeedUpdate(true),
    m_isPressed(false)
{
    m_sprite.setOrigin(sf::Vector2f(texture->getSize()) * 0.5f);
    m_sprite.setColor(m_normalColor);
}


void Button::update(const sf::Vector2f& mousePosition, bool isClicked) noexcept
{
    if(m_boundsNeedUpdate)
    {
        m_bounds = m_sprite.getGlobalBounds();
        m_boundsNeedUpdate = false;
    }

    sf::Color color { m_normalColor };
    const bool isUnderCursor = m_bounds.contains(mousePosition);
    m_isPressed = false;

    if(isClicked)
    {
        if(isUnderCursor)
        {
            color = m_clickedColor;
            m_isPressed = true;
        }
    }
    else
    {
        if(isUnderCursor)
            color = m_underCursorColor;
    }

    m_sprite.setColor(color);
}


void Button::setPosition(const sf::Vector2f& position) noexcept
{
    m_sprite.setPosition(position);
    m_boundsNeedUpdate = true;
}


void Button::setSize(const sf::Vector2f& newSize) noexcept
{
    if (auto& rect = m_sprite.getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
    {
        float dx = newSize.x / rect.size.x;
        float dy = newSize.y / rect.size.y;
        m_sprite.setScale({dx, dy});
        m_boundsNeedUpdate = true;
    }
}


bool Button::isPressed() const noexcept
{
    return m_isPressed;
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.blendMode = sf::BlendAlpha;
    target.draw(m_sprite, states);
}