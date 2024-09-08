#ifndef ECS_TYPES_HPP
#define ECS_TYPES_HPP

#include <cstdint>

#include "common/Defines.hpp"

BEGIN_NAMESPACE_ECS

using Entity        = uint32_t;
using ComponentId   = uint32_t;
using ComponentType = uint32_t;
using EntitySetType = size_t;

END_NAMESPACE_ECS

#endif // !ECS_TYPES_HPP
