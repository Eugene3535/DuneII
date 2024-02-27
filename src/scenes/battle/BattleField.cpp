#include "utils/FileProvider.hpp"
#include "game/Game.hpp"
#include "states/GameState.hpp"
#include "scenes/battle/BattleField.hpp"

BattleField::BattleField() noexcept
{

}

BattleField::~BattleField()
{
}

// TODO : add unload tilemap functional
bool BattleField::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    m_isLoaded = m_tilemap.loadFromFile(FileProvider::getPathToFile(info));

    return m_isLoaded;
}

void BattleField::open() noexcept
{

}

void BattleField::close() noexcept
{

}

void BattleField::update(sf::Time dt) noexcept
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_viewport.move(-10, 0);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_viewport.move(10, 0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_viewport.move(0, -10);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_viewport.move(0, 10);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
        GameState::A_SCENE_NEEDS_TO_BE_CHANGED = true;
        GameState::next_scene = GameState::MAIN_MENU;
    }
}

void BattleField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& layer : m_tilemap.m_layers)
    {
        target.draw(layer.vertexBuffer, layer.texture);
    }
}