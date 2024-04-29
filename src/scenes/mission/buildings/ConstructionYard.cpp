#include "scenes/mission/buildings/ConstructionYard.hpp"

ConstructionYard::ConstructionYard() noexcept:
    Building()
{
}

ConstructionYard::~ConstructionYard()
{
}

void ConstructionYard::update(std::int32_t dt) noexcept
{

}

void ConstructionYard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices, 12, sf::Triangles, states);
}