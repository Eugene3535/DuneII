#include "scenes/mission/buildings/Building.hpp"

Building::Building() noexcept:
    m_type(Building::CONCRETE_SLAB),
    m_hitPoints(0),
    m_maxHitPoints(0),
    m_cost(0),
    m_isEnemy(false)
{

}

Building::~Building()
{
    
}

void Building::construct(const Building::Data* data) noexcept
{
    m_type         = data->type;
    m_bounds       = data->globalBounds;
    m_cost         = data->cost;
    m_hitPoints    = data->hitPoints;
    m_maxHitPoints = data->maxHitPoints;
}

void Building::repair(int points) noexcept
{
    if( (m_type != Building::CONCRETE_SLAB) && (m_type != Building::WALL) )
    {
        m_hitPoints += points;

        if(m_hitPoints > m_maxHitPoints)
            m_hitPoints = m_maxHitPoints;
    }
}

void Building::damage(int points) noexcept
{
    m_hitPoints -= points;
}

Building::Type Building::type() const noexcept
{
    return m_type;
}

const sf::IntRect& Building::bounds() const noexcept
{
    return m_bounds;
}

int Building::hitPoints() const noexcept
{
    return (m_hitPoints > 0) ? m_hitPoints : 0;
}

int Building::cost() const noexcept
{
    return m_cost;
}

bool Building::isEnemy() const noexcept
{
    return m_isEnemy;
}

bool Building::isDestroyed() const noexcept
{
    return (m_hitPoints < 1);
}