#ifndef ANIMATION_DATA_HPP
#define ANIMATION_DATA_HPP

#include <cstdint>
#include <string_view>

#include "ecs/components/Animation.hpp"

class AnimationData
{
public:
    enum Layout
    {
        SINGLE,
        LINEAR,
        GRID
    } layout;

    const sf::Texture* texture = nullptr;
    sf::IntRect        startFrame;
    std::string_view   name;
    uint32_t           columns = 0u;
    uint32_t           rows = 0u;
    uint32_t           duration = 0u;
    uint32_t           delay = 0u;
};

#endif //!ANIMATION_DATA_HPP