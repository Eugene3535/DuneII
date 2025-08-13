#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <cstdint>
#include <span>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Animation:
    public sf::Sprite
{
public:
    Animation(const sf::Texture* texture) noexcept;
    Animation(const sf::Texture* texture, const sf::IntRect& startFrame) noexcept;

    std::span<const sf::IntRect> frames;
    uint32_t currentFrame;
    sf::Time delay;
    sf::Time timer;
    bool isLooped;
    bool isOver;
};

#endif // !ANIMATION_HPP