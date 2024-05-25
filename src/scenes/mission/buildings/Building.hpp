#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics.hpp>

#include "utils/Defines.hpp"

// More information is available here: https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

class Building final:
    public sf::Sprite
{
public:
    enum Type: int32_t
    {
        NONE,
        CONCRETE_SLAB,
        CONSTRUCTION_YARD,
        SPICE_SILOS,
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
    struct Data
    {
        sf::IntRect    localBounds{};
        sf::IntRect    globalBounds{};
        Building::Type type { Building::NONE };
        int32_t        cost         { 0 };
        int32_t        hitPoints    { 0 };
        int32_t        maxHitPoints { 0 };
        
        bool hasAnAnimatedDynastyFlag { false };
        bool hasAnAnimatedLandingPad  { false };
        bool isEnemy                  { false };
    };

public:
    Building() noexcept;
    ~Building();

    void construct(const struct Building::Data* data) noexcept;

    void repair(int32_t points) noexcept;
    void damage(int32_t points) noexcept;

    Type               type()      const noexcept;
    const sf::IntRect& bounds()    const noexcept;
    int32_t            hitPoints() const noexcept;
    int32_t            cost()      const noexcept;

    bool isEnemy() const noexcept;
    bool isDestroyed() const noexcept;

protected:
    Type m_type;

protected:
    sf::IntRect m_bounds;

protected:
    int32_t m_hitPoints;
    int32_t m_maxHitPoints;
    int32_t m_cost;
    bool    m_isEnemy;
};

#endif // !BUILDING_HPP