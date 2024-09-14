#ifndef COMPONENT_TABLE_HPP
#define COMPONENT_TABLE_HPP

#include <cstdint>

#include "common/Defines.hpp"

struct ComponentTable
{
    int32_t position  = UNDEFINED_COMPONENT;
    int32_t velocity  = UNDEFINED_COMPONENT;
    int32_t animation = UNDEFINED_COMPONENT;
    int32_t name      = UNDEFINED_COMPONENT;
};

#endif // !COMPONENT_TABLE_HPP