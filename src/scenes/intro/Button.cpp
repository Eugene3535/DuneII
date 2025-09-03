#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "scenes/intro/Button.hpp"


namespace
{// set size in pixels
    void set_button_size(sf::Sprite& button, const sf::Vector2f& newSize) noexcept
    {
        if (auto& rect = button.getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
        {
            float dx = newSize.x / rect.size.x;
            float dy = newSize.y / rect.size.y;
            button.setScale({dx, dy});
        }
    }
}


Button::Button(const sf::Texture* texture, const sf::IntRect& bounds) noexcept:
    m_sprite(*texture),
    m_relativePosition(bounds.position),
    m_size(bounds.size)
{
    m_sprite.setOrigin(sf::Vector2f(texture->getSize()) * 0.5f);
    m_sprite.setPosition(sf::Vector2f(bounds.position));
    m_sprite.setColor(s_normalColor);
    set_button_size(m_sprite, sf::Vector2f(bounds.size));
}


void Button::update(const sf::View& view, const sf::Vector2i& mousePosition) noexcept
{
    const auto position = m_sprite.getPosition();
    const auto center   = view.getCenter();
    const auto size     = view.getSize();

    
}


bool Button::hasClicked() const noexcept
{
    return false;
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.blendMode = sf::BlendAlpha;
    target.draw(m_sprite, states);
}