#include "scenes/mission/buildings/Building.hpp"

Building::Building() noexcept:
    m_armor(0),
    m_maxArmor(0),
    m_cost(0),
    m_isEnemy(true)
{
}

Building::~Building()
{
}

void Building::update(std::int32_t dt) noexcept
{

}

void Building::repair(std::int32_t value) noexcept
{
    if(m_armor + value <= m_maxArmor)
        m_armor += value;
}

void Building::damage(std::int32_t value) noexcept
{
    m_armor -= value;
}

std::int32_t Building::armor() const noexcept
{
    return (m_armor > -1) ? m_armor : 0;
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
    return (m_armor > 0);
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}