#ifndef GRAPHICS_UTILS_HPP
#define GRAPHICS_UTILS_HPP

#include <SFML/Graphics/Rect.hpp>

namespace sf
{
    class Sprite;
}

class GraphicsUtils
{
public:
    static void setSpriteSize(class sf::Sprite& sprite, float width, float height) noexcept;
    static void setSpriteSize(class sf::Sprite& sprite, const sf::Vector2i& size) noexcept;

    static sf::Vector2i getSpriteSize(const class sf::Sprite& sprite) noexcept;
};

#endif // !GRAPHICS_UTILS_HPP