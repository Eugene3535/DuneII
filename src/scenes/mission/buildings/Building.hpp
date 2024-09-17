#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics/Sprite.hpp>

#include "common/Enums.hpp"

// More information is available here: https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

class Building final:
    public sf::Sprite
{
    friend class Builder;

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

public:
    Building() noexcept;
    ~Building();

    void repair(int32_t points) noexcept;
    void damage(int32_t points) noexcept;
    void changeOwner(House house) noexcept;

    Type               getType()         const noexcept;
    House              getOwner()        const noexcept;
    const sf::IntRect& getBounds()       const noexcept;
    int32_t            getHitPoints()    const noexcept;
    int32_t            getMaxHitPoints() const noexcept;

    bool isDestroyed() const noexcept;

private:
    Type m_type;
    House m_owner;

    sf::IntRect m_bounds;
    int32_t     m_hitPoints;
    int32_t     m_maxHitPoints;

    friend class TileMap;
};

#endif // !BUILDING_HPP