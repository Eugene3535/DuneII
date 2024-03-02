#ifndef BATTLE_FIELD_HPP
#define BATTLE_FIELD_HPP

#include "scenes/mission/TileMap.hpp"
#include "scenes/Scene.hpp"

class Mission:
    public Scene
{
public:
    Mission(class Game* game) noexcept;
    ~Mission();

    bool load(const std::string& info) noexcept override;
    void update(sf::Time dt)           noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    TileMap m_tilemap;
};

#endif // !BATTLE_FIELD_HPP