#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics/Sprite.hpp>

class Button:
    public sf::Drawable
{
public:
    Button(const sf::Texture* texture) noexcept;

    void update(const sf::Vector2f& mousePosition, bool isClicked) noexcept;

    void setPosition(const sf::Vector2f& position) noexcept;
    void setSize(const sf::Vector2f& newSize) noexcept;

    bool isPressed() const noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    static constexpr sf::Color m_normalColor      { 150, 150, 150, 150 };
    static constexpr sf::Color m_underCursorColor { 200, 200, 200, 200 };
    static constexpr sf::Color m_clickedColor     { sf::Color::White   };

    sf::Sprite m_sprite;
    sf::FloatRect m_bounds;
    bool m_boundsNeedUpdate;
    bool m_isPressed;
};

#endif // !BUTTON_HPP