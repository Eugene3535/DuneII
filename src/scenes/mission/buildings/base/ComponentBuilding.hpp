#include "scenes/mission/buildings/base/Building.hpp"

/// \brief
/// Buildings can consist of one, two or three drawable components(for example, an enrichment plant consists of a main building sprite, 
/// an animated dynasty flag and a landing pad, and a turret is a single animated sprite). 
/// We define for each component building type a different number of vertices and a different drawing function to optimize the number of calls to sf::RenderTarget::draw()

class OnePartBuilding:
    public Building
{
public:
    OnePartBuilding() noexcept;
    ~OnePartBuilding();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
    sf::Vertex m_vertices[ONE_COMPONENT_BUILDING_VERTEX_COUNT];
};

class TwoPartBuilding:
    public Building
{
public:
    TwoPartBuilding() noexcept;
    ~TwoPartBuilding();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
    sf::Vertex m_vertices[TWO_COMPONENT_BUILDING_VERTEX_COUNT];
};

class ThreePartBuilding:
    public Building
{
public:
    ThreePartBuilding() noexcept;
    ~ThreePartBuilding();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
    sf::Vertex m_vertices[THREE_COMPONENT_BUILDING_VERTEX_COUNT];
};

