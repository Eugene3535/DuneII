#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <cstdint>

#include <SFML/Graphics/Rect.hpp>

struct Position
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Velocity
{
    float dx = 0.0f;
    float dy = 0.0f;
};

struct Animation
{
    const sf::IntRect* frames = nullptr;
    uint32_t           duration = 0u;
    float              timer = 0.0f;
    float              fps = 0.0f;
};

#endif // !COMPONENTS_HPP