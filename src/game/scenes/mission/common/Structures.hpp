#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include <cstdint>


struct Structure
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
    };

    Type type;
    int32_t cost;
    int32_t armor;
};


struct IConstructionSite
{
    char* tileMask;
    int32_t width;
    int32_t height;
};

#endif // !STRUCTURE_HPP