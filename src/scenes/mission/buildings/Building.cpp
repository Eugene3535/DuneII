#include "scenes/mission/buildings/Building.hpp"

Building::Building() noexcept:
    m_type(Building::CONCRETE_SLAB),
    m_armor(0),
    m_maxArmor(0),
    m_cost(0),
    m_isEnemy(false)
{
}

Building::~Building()
{
}

void Building::construct(const Building::Data* data) noexcept
{
    m_type = data->type;
    m_cost = data->cost;
}

void Building::repair(std::int32_t value) noexcept
{
    if( (m_type != Building::CONCRETE_SLAB) && (m_type != Building::WALL) )
    {
        m_armor += value;

        if(m_armor > m_maxArmor)
            m_armor = m_maxArmor;
    }
}

void Building::damage(std::int32_t value) noexcept
{
    m_armor -= value;
}

Building::Type Building::type() const noexcept
{
    return m_type;
}

std::int32_t Building::armor() const noexcept
{
    return (m_armor > 0) ? m_armor : 0;
}

std::int32_t Building::cost() const noexcept
{
    return m_cost;
}

bool Building::isEnemy() const noexcept
{
    return m_isEnemy;
}

bool Building::isDestroyed() const noexcept
{
    return (m_armor < 1);
}