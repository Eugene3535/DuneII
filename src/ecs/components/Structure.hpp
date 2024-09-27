#ifndef BUILDING_DATA_HPP
#define BUILDING_DATA_HPP

#include "ecs/common/Component.hpp"
#include "common/Enums.hpp"

// More information is available here: https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

BEGIN_NAMESPACE_ECS

class Structure: public ecs::Component<Structure>
{
public:
    StructureType type         = StructureType::INVALID;
    HouseType     owner        = HouseType::INVALID;
    int32_t       hitPoints    = 0;
    int32_t       maxHitPoints = 0;
};

END_NAMESPACE_ECS

#endif // !BUILDING_DATA_HPP