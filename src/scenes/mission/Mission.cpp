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
        m_view_position.x -= 10;
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_view_position.x += 10;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_view_position.y -= 10;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_view_position.y += 10;

    viewport.setCenter(m_view_position + (viewport.getSize() * 0.5f));

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