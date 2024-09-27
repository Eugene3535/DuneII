#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics/Sprite.hpp>

#include "common/Enums.hpp"

// More information is available here: https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

class Building:
    public sf::Sprite
{
public:
    enum Type: int32_t
    {
        NONE,
        CONCRETE_SLAB,
        CONSTRUCTION_YARD,
        SPICE_SILO,
        STARPORT,
        WIND_TRAP,
        SPICE_REFINERY,
        RADAR_OUTPOST,
        REPAIR_FACILITY,
        PALACE,
        HIGH_TECH_FACILITY,
        BARRACKS,
        VEHICLE_FACTORY,
        WALL,
        TURRET,
        ROCKET_TURRET
    };

    Type        type = Type::NONE;
    House       owner = House::Fremen;
    sf::IntRect bounds;
    int32_t     hitPoints = 0;
    int32_t     maxHitPoints = 0;
};

#endif // !BUILDING_HPP