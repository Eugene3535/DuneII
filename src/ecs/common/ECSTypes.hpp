#ifndef ECS_TYPES_HPP
#define ECS_TYPES_HPP

#include <cstdint>

#include "common/Defines.hpp"

BEGIN_NAMESPACE_ECS

using entity_t       = uint32_t;
using component_id_t = uint32_t;
using component_t    = uint32_t;
using entity_set_t   = size_t;

END_NAMESPACE_ECS

#endif // !ECS_TYPES_HPP
