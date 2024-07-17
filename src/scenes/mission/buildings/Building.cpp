#include "scenes/mission/buildings/Building.hpp"

Building::Building() noexcept:
    m_type(Building::CONCRETE_SLAB),
    m_hitPoints(0),
    m_maxHitPoints(0),
    m_isEnemy(false)
{

}

Building::~Building()
{
    
}

void Building::repair(int32_t points) noexcept
{
    if( ( m_type != Building::CONCRETE_SLAB) && (m_type != Building::WALL) )
    {
        m_hitPoints += points;

        if(m_hitPoints > m_maxHitPoints)
            m_hitPoints = m_maxHitPoints;
    }
}

void Building::damage(int32_t points) noexcept
{
    m_hitPoints -= points;
}

Building::Type Building::type() const noexcept
{
    return m_type;
}

const sf::Rect<uint8_t>& Building::bounds() const noexcept
{
    return m_bounds;
}

int32_t Building::hitPoints() const noexcept
{
    return (m_hitPoints > 0) ? m_hitPoints : 0;
}

bool Building::isEnemy() const noexcept
{
    return m_isEnemy;
}

bool Building::isDestroyed() const noexcept
{
    return (m_hitPoints < 1);
}