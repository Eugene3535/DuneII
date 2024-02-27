#ifndef BATTLE_FIELD_HPP
#define BATTLE_FIELD_HPP

#include "scenes/battle/TileMap.hpp"
#include "scenes/Scene.hpp"

class BattleField:
    public Scene
{
public:
    BattleField() noexcept;
    ~BattleField();

    bool load(const std::string& info) noexcept override;
    void open()                        noexcept override;
    void close()                       noexcept override;
    void update(sf::Time dt)           noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    TileMap m_tilemap;
};

#endif // !BATTLE_FIELD_HPP