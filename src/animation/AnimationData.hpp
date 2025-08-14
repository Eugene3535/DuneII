#ifndef ANIMATION_DATA_HPP
#define ANIMATION_DATA_HPP

#include <cstdint>
#include <string_view>

#include <SFML/System/Time.hpp>

#include "ecs/components/Animation.hpp"

class AnimationData
{
public:
    enum Layout
    {
        SINGLE = 0,
        LINEAR,
        GRID
    } layout;

    sf::IntRect      startFrame;
    std::string_view name;
    uint32_t         columns    = 0u;
    uint32_t         rows       = 0u;
    uint32_t         duration   = 0u;
    sf::Time         delay;
    bool             isLooped   = false;
};

#endif //!ANIMATION_DATA_HPP