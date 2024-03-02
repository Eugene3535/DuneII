#include "utils/FileProvider.hpp"
#include "game/Game.hpp"
#include "scenes/mission/Mission.hpp"

Mission::Mission(Game* game) noexcept:
    Scene(game)
{

}

Mission::~Mission()
{
}

// TODO : add unload tilemap functional
bool Mission::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    m_isLoaded = m_tilemap.loadFromFile(FileProvider::getPathToFile(info));

    return m_isLoaded;
}

void Mission::update(sf::Time dt) noexcept
{
    auto& viewport = m_game->viewport;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        viewport.move(-10, 0);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        viewport.move(10, 0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        viewport.move(0, -10);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        viewport.move(0, 10);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
        m_game->a_scene_needs_to_be_changed = true;
        m_game->next_scene = Game::GameScene::MAIN_MENU;
    }
}

void Mission::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& layer : m_tilemap.m_layers)
    {
        target.draw(layer.vertexBuffer, layer.texture);
    }
}