#ifndef ACTION_DATA_HPP
#define ACTION_DATA_HPP

#include "game/scenes/mission/common/Structures.hpp"


namespace Action
{
    
struct Construction
{
    HouseType owner;
    ivec2s area; // in tiles, for example 3x2
    float duration;
    int32_t cost;
    int32_t armor;
};

}

#endif // !ACTION_DATA_HPP