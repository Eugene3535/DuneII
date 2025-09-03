#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics/Sprite.hpp>

class Button:
    public sf::Drawable
{
public:
    Button(const sf::Texture* texture, const sf::IntRect& bounds) noexcept;

    void update(const class sf::View& view, const sf::Vector2i& mousePosition) noexcept;
    bool hasClicked() const noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite m_sprite;

    sf::Vector2f m_relativePosition;
    sf::Vector2f m_size;

    const sf::Color s_normalColor      { 150, 150, 150, 150 };
    const sf::Color s_underCursorColor { 200, 200, 200, 200 };
    const sf::Color s_clickedColor     { sf::Color::White   };
};

#endif // !BUTTON_HPP