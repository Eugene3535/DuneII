#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include "common/Enums.hpp"

// More information is available here: https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

class Structure
{
public:
    StructureType type          = StructureType::INVALID;
    HouseType     owner         = HouseType::INVALID;
    int32_t       hitPoints     = 0;
    int32_t       maxHitPoints  = 0;
};

#endif // !STRUCTURE_HPP