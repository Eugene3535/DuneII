#ifndef BATTLE_FIELD_HPP
#define BATTLE_FIELD_HPP

#include "scenes/SceneNode.hpp"

class BattleField:
    public SceneNode
{
public:
    BattleField(SceneNode* root) noexcept;
    ~BattleField();

    bool load()           noexcept override;
    void update(float dt) noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite m_player;
};

#endif // !BATTLE_FIELD_HPP