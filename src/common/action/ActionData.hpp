#pragma once

#include <cglm/struct/ivec2.h>

#include "common/Defines.hpp"

BEGIN_NAMESPACE_ACTION
    
struct Construction
{
    float duration; // in seconds
    float countdown;
    float* progress;
};


struct CollisionMask
{
    char* mask;
    uint32_t mapWidth;
};


struct Movement
{
    struct
    {
        ivec2s src;
        ivec2s dest;
        vec2s cur;
    } position;
    
    float timer;
    float velocity;
    
    CollisionMask* mask;
};

END_NAMESPACE