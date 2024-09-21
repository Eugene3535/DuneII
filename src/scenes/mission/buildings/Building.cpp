#include <cstdint>

#include "scenes/mission/buildings/Building.hpp"

Building::Building() noexcept:
    m_type(Building::NONE),
    m_owner(House::Fremen),
    m_bounds(),
    m_hitPoints(0),
    m_maxHitPoints(0)
{

}

Building::~Building()
{
    
}

void Building::repair(int32_t points) noexcept
{
    const bool repairable = ((m_type != Building::CONCRETE_SLAB) && (m_type != Building::WALL));

    if( repairable )
    {
        m_hitPoints += points;

        if(m_hitPoints > m_maxHitPoints)
            m_hitPoints = m_maxHitPoints;
    }
}

void Building::damage(int32_t points) noexcept
{
    m_hitPoints -= points;

    if(m_hitPoints < 0)
        m_hitPoints = 0;
}

void Building::changeOwner(House house) noexcept
{
    m_owner = house;
}

Building::Type Building::getType() const noexcept
{
    return m_type;
}

House Building::getOwner() const noexcept
{
    return m_owner;
}

const sf::IntRect& Building::getBounds() const noexcept
{
    return m_bounds;
}

int32_t Building::getHitPoints() const noexcept
{
    return m_hitPoints;
}

int32_t Building::getMaxHitPoints() const noexcept
{
    return m_maxHitPoints;
}

bool Building::isDestroyed() const noexcept
{
    return (m_hitPoints < 1);
}