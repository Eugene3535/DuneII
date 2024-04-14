#include "utils/FileProvider.hpp"
#include "game/Game.hpp"
#include "scenes/mission/Mission.hpp"

Mission::Mission(Game& game) noexcept:
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

    m_isLoaded = m_tilemap.loadFromFile(FileProvider().find_path_to_file(info));

    return m_isLoaded;
}

void Mission::update(sf::Time dt) noexcept
{
    if(!m_isLoaded)
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_view_position.x -= 10;
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_view_position.x += 10;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_view_position.y -= 10;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_view_position.y += 10;

    auto& viewport  = m_game.viewport;
    auto view_size  = viewport.getSize();
    auto map_width  = m_tilemap.m_mapSize.x * m_tilemap.m_tileSize.x;
    auto map_height = m_tilemap.m_mapSize.y * m_tilemap.m_tileSize.y;
    
    if(m_view_position.x < 0) m_view_position.x = 0;
    if(m_view_position.y < 0) m_view_position.y = 0;
    if(m_view_position.x + view_size.x > map_width)  m_view_position.x = map_width - view_size.x;
    if(m_view_position.y + view_size.y > map_height) m_view_position.y = map_height - view_size.y;

    viewport.setCenter(m_view_position + (view_size * 0.5f));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
        m_game.sceneNeedToBeChanged = true;
        m_game.next_scene = Game::GameScene::MAIN_MENU;
    }
}

void Mission::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& layer : m_tilemap.m_layers)
    {
        target.draw(layer.vertexBuffer, layer.texture);
    }
}