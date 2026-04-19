#ifndef ACTION_DATA_HPP
#define ACTION_DATA_HPP

#include "game/scenes/mission/common/Structures.hpp"

#define BEGIN_NAMESPACE_ACTION namespace Action {
#define END_NAMESPACE_ACTION }


BEGIN_NAMESPACE_ACTION

struct Construction
{
    HouseType owner;
    ivec2s area; // in tiles, for example 3x2
    float duration;
    int32_t cost;
    int32_t armor;
};

END_NAMESPACE_ACTION

#endif // !ACTION_DATA_HPP