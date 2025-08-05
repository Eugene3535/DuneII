#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <cstdint>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Animation:
    public sf::Sprite
{
public:
    Animation(const sf::Texture* texture) noexcept;
    Animation(const sf::Texture* texture, const sf::IntRect& startFrame) noexcept;

    const sf::IntRect* frames;
    uint32_t           duration;
    uint32_t           currentFrame;
    sf::Time           delay;
    sf::Time           timer;
    bool               isLooped;
    bool               isOver;
};

#endif // !ANIMATION_HPP