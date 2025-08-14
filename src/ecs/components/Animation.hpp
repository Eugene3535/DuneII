#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <cstdint>
#include <span>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>


struct Animation
{
    std::span<const sf::IntRect> frames;
    uint32_t currentFrame = 0;
    sf::Time delay;
    sf::Time timer;
    bool isLooped = false;
    bool isOver = false;
};

#endif // !ANIMATION_HPP