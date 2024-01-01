#ifndef BATTLE_FIELD_HPP
#define BATTLE_FIELD_HPP

#include "scenes/battle/TileMap.hpp"
#include "scenes/SceneNode.hpp"

class BattleField:
    public SceneNode
{
public:
    BattleField(SceneNode* root) noexcept;
    ~BattleField();

    void update(float dt) noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    TileMap m_tilemap;
};

#endif // !BATTLE_FIELD_HPP