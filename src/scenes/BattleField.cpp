#include "utils/Resources.hpp"
#include "game/Game.hpp"
#include "scenes/BattleField.hpp"

BattleField::BattleField(SceneNode* root) noexcept:
    SceneNode(root)
{
    m_title = "Battle field";
}

BattleField::~BattleField()
{
}

bool BattleField::load() noexcept
{
    if(m_isDone)
        return false;

    sf::Texture* texture = GetTexture("red");

    if(!texture)
        return false;

    
    auto size = texture->getSize();
    m_player.setTexture(*texture);
    m_player.setColor(sf::Color::Red);
    m_player.setOrigin(size.x >> 1, size.y >> 1);

    return true;
}

void BattleField::update(float dt) noexcept
{
    if(m_isDone)
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_player.move(-10, 0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_player.move(10, 0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_player.move(0, -10);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_player.move(0, 10);
}

void BattleField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isDone)
        return;
        
    target.draw(m_player);
}