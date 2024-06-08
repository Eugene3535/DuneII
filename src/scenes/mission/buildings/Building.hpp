#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics.hpp>

// More information is available here: https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

class Building final:
    public sf::Sprite
{
public:
    enum Type: int
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
    struct Data
    {
        sf::IntRect    textureRect{};
        sf::IntRect    globalBounds{};
        Building::Type type { Building::NONE };
        int cost         { 0 };
        int hitPoints    { 0 };
        int maxHitPoints { 0 };
        
        bool hasAnAnimatedDynastyFlag { false };
        bool hasAnAnimatedLandingPad  { false };
        bool isEnemy                  { false };
    };

public:
    Building() noexcept;
    ~Building();

    void construct(const Data* data) noexcept;

    void repair(int points) noexcept;
    void damage(int points) noexcept;

    Type               type()      const noexcept;
    const sf::IntRect& bounds()    const noexcept;
    int            hitPoints() const noexcept;
    int            cost()      const noexcept;

    bool isEnemy() const noexcept;
    bool isDestroyed() const noexcept;

protected:
    Type m_type;

protected:
    sf::IntRect m_bounds;

protected:
    int  m_hitPoints;
    int  m_maxHitPoints;
    int  m_cost;
    bool m_isEnemy;
};

#endif // !BUILDING_HPP