#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <cstdint>

#include <SFML/Graphics/Texture.hpp>

class Animation
{
public:
    const sf::Texture* texture  = nullptr;
    const sf::IntRect* frames   = nullptr;
    uint32_t           duration = 0; // in frames
    uint32_t           delay    = 0; // in milliseconds
    uint32_t           timer    = 0; // in milliseconds
};

#endif // !ANIMATION_HPP