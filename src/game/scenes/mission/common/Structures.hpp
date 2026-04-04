#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include <cglm/struct/ivec2.h>

#include "common/Enums.hpp"

// More info is available here
// https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II
struct StructureInfo
{
    enum Type : int32_t
    {
        SLAB_1x1 = 0,
        SLAB_2x2,
        PALACE,
        VEHICLE,
        HIGH_TECH,
        CONSTRUCTION_YARD,
        WIND_TRAP,
        BARRACKS,
        STARPORT,
        REFINERY,
        REPAIR,
        WALL,
        TURRET,
        ROCKET_TURRET,
        SILO,
        OUTPOST,
        MAX,
        INVALID = 0xFF
    } type;

    HouseType owner;

    uint32_t id;
    uint32_t frame;
    ivec2s texOffset;

    int32_t cost;
    int32_t armor;
    int32_t maxArmor;

    bool hasLandingPad;
    bool hasFlag;
    bool isEnabled;
};

#endif // !STRUCTURE_HPP