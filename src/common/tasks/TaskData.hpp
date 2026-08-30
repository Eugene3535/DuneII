#pragma once

#include <cglm/struct/ivec2.h>

#include "common/Defines.hpp"
#include "common/Enums.hpp"


BEGIN_NAMESPACE_TASK
    
struct Construction
{
    EntityIcon icon;
    float duration; // in seconds
    float* progress;
};


END_NAMESPACE