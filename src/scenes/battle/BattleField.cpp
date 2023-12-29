#include "utils/FileProvider.hpp"
#include "game/Game.hpp"
#include "scenes/battle/BattleField.hpp"

BattleField::BattleField(SceneNode* root) noexcept:
    SceneNode(root)
{
    m_state = SceneNode::State::BATTLE;
}

BattleField::~BattleField()
{
}

bool BattleField::load() noexcept
{
    if(m_isDone)
        return false;

    if(m_tilemap.loadFromFile(FileProvider::getPathToFile("Atreides8.tmx")))
    {
        m_isLoaded = true;

        return m_isLoaded;
    }

    return false;
}

void BattleField::update(float dt) noexcept
{
    if(m_isDone)
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_viewport.move(-10, 0);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_viewport.move(10, 0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_viewport.move(0, -10);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_viewport.move(0, 10);
}

void BattleField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isDone)
        return;

    for (auto& layer : m_tilemap.m_layers)
    {
        target.draw(layer.vertexBuffer, layer.texture);
    }
}