#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "scenes/mission/animated_tile/AnimatedTile.hpp"

// More information is available here: https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

class Building final:
    public AnimatedTile
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

public:
    Building() noexcept;
    ~Building();

    void repair(int32_t points) noexcept;
    void damage(int32_t points) noexcept;

    Type               type()      const noexcept;
    const sf::Rect<uint8_t>& bounds()    const noexcept;
    int32_t            hitPoints() const noexcept;

    bool isEnemy() const noexcept;
    bool isDestroyed() const noexcept;

private:
    Type m_type;
    sf::Rect<uint8_t> m_bounds;
    int32_t  m_hitPoints;
    int32_t  m_maxHitPoints;
    bool     m_isEnemy;

    friend class TileMap;
};

#endif // !BUILDING_HPP