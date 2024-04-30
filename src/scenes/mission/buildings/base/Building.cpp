#include "scenes/mission/buildings/base/BuildingData.hpp"
#include "scenes/mission/buildings/base/Building.hpp"

Building::Building() noexcept:
    m_type(Building::CONCRETE_SLAB),
    m_textureRect(),
    m_texture(nullptr),
    m_armor(0),
    m_maxArmor(0),
    m_cost(0),
    m_isEnemy(false)
{
}

Building::~Building()
{
}

void Building::construct(const BuildingData* data) noexcept
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

Building::Type Building::type() const noexcept
{
    return m_type;
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

void Building::setTextureRect(const sf::IntRect& rectangle) noexcept
{
    m_textureRect = rectangle;

    const float width  = static_cast<float>(m_textureRect.width);
    const float height = static_cast<float>(m_textureRect.height);
    const float left   = static_cast<float>(m_textureRect.left);
    const float right  = left + width;
    const float top    = static_cast<float>(m_textureRect.top);
    const float bottom = top + height;

    m_vertices[0].position = sf::Vector2f(0.0f, 0.0f);
    m_vertices[1].position = sf::Vector2f(0.0f, height);
    m_vertices[2].position = sf::Vector2f(width, 0.0f);
    m_vertices[3].position = sf::Vector2f(width, height);

    m_vertices[0].texCoords = sf::Vector2f(left, top);
    m_vertices[1].texCoords = sf::Vector2f(left, bottom);
    m_vertices[2].texCoords = sf::Vector2f(right, top);
    m_vertices[3].texCoords = sf::Vector2f(right, bottom);
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = m_texture;
    target.draw(m_vertices, 4, sf::TriangleStrip, states);
}