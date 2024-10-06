#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <cstdint>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Texture.hpp>

class Animation
{
public:
    const sf::Texture* texture      = nullptr;
    const sf::IntRect* frames       = nullptr;
    uint32_t           duration     = 0;
    uint32_t           currentFrame = 0;
    sf::Time           delay;
    sf::Time           timer;
    bool               isLooped     = false;
    bool               isOver       = false;
};

#endif // !ANIMATION_HPP