#ifndef COMPONENT_TABLE_HPP
#define COMPONENT_TABLE_HPP

#include "managers/ecs/common/Components.hpp"

BEGIN_NAMESPACE_ECS

struct ComponentTable
{
    int32_t position  = UNDEFINED_COMPONENT;
    int32_t velocity  = UNDEFINED_COMPONENT;
    int32_t animation = UNDEFINED_COMPONENT;
    int32_t name      = UNDEFINED_COMPONENT;
};

END_NAMESPACE_ECS

#endif // !COMPONENT_TABLE_HPP