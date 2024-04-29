#include "scenes/mission/buildings/base/ComponentBuilding.hpp"

////////////////////////////////////////////////////////////
OnePartBuilding::OnePartBuilding() noexcept:
    Building()
{

}

OnePartBuilding::~OnePartBuilding()
{

}

void OnePartBuilding::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = m_texture;
    target.draw(m_vertices, ONE_COMPONENT_BUILDING_VERTEX_COUNT, sf::TrianglesFan, states);
}

////////////////////////////////////////////////////////////
TwoPartBuilding::TwoPartBuilding() noexcept:
    Building()
{

}

TwoPartBuilding::~TwoPartBuilding()
{

}

void TwoPartBuilding::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = m_texture;
    target.draw(m_vertices, TWO_COMPONENT_BUILDING_VERTEX_COUNT, sf::Triangles, states);
}

////////////////////////////////////////////////////////////
ThreePartBuilding::ThreePartBuilding() noexcept:
    Building()
{

}

ThreePartBuilding::~ThreePartBuilding()
{

}

void ThreePartBuilding::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = m_texture;
    target.draw(m_vertices, THREE_COMPONENT_BUILDING_VERTEX_COUNT, sf::Triangles, states);
}