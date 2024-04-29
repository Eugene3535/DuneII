#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics.hpp>

#include "utils/Defines.hpp"

// More information is available here: https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

class Building:
    public sf::Drawable
{
public:
    enum Type: std::int32_t
    {
        CONCRETE_SLAB,
        CONSTRUCTION_YARD,
        SPICE_SILO,
        STARPORT,
        WINDTRAP,
        REFINERY,
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
    virtual ~Building();

    virtual void update(std::int32_t dt) noexcept;

    void repair(std::int32_t value) noexcept;
    void damage(std::int32_t value) noexcept;

    std::int32_t armor() const noexcept;
    std::int32_t cost() const noexcept;

    bool isEnemy() const noexcept;
    bool isDestroyed() const noexcept;

protected:
    const sf::Texture* m_texture; 

protected:
    std::int32_t m_armor;
    std::int32_t m_maxArmor;
    std::int32_t m_cost;
    bool         m_isEnemy;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // !BUILDING_HPP