#pragma once

#include <cglm/struct/ivec2.h>

#include "common/Enums.hpp"

// More info is available here
// https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II
struct StructureInfo
{
    enum class Type : int32_t
    {
        Slab_1x1 = 0,
        Slab_2x2,
        Palace,
        Vehicle,
        HighTech,
        ConstructionYard,
        WindTrap,
        Barracks,
        Starport,
        Refinery,
        Repair,
        Wall,
        Turret,
        RocketTurret,
        Silo,
        Outpost,
        Max,
        Undefined = 0xFF
    } type;

    HouseType owner;

    uint32_t stride; // in vertex array
    uint32_t frame;  // start index for draw call

    int32_t cost;
    int32_t armor;
    int32_t maxArmor;

 // construction mode
    EntityIcon icon;
    float duration;
    float progress;
    bool isUnderConstruction;
};