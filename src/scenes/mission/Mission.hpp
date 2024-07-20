#ifndef MISSION_HPP
#define MISSION_HPP

#include "scenes/mission/tilemap/TileMap.hpp"
#include "scenes/Scene.hpp"

class Mission:
    public Scene
{
public:
    Mission(struct Game& game) noexcept;
    ~Mission();

    bool load(const std::string& info) noexcept override;
    void update(sf::Time dt)           noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    TileMap m_tilemap;
    std::vector<Building*> m_buildings;

    sf::Vector2f m_view_position;
};

#endif // !MISSION_HPP