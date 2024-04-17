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

    m_isLoaded = m_tilemap.loadFromFile(FileProvider().findPathToFile(info));

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
    auto map_width  = m_tilemap.mapSize.x * m_tilemap.tileSize.x;
    auto map_height = m_tilemap.mapSize.y * m_tilemap.tileSize.y;
    
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
    if(m_isLoaded)
    {
        const auto& vertexBuffer = m_tilemap.landscape.vertices;
        states.texture = m_tilemap.landscape.texture;
        target.draw(vertexBuffer, states);

        // const auto* vertices = m_tilemap.staticBuildings.vertices.data();
        // states.texture = m_tilemap.staticBuildings.texture;
        // std::size_t vertexCount = m_tilemap.staticBuildings.vertices.size();
        // target.draw(vertices, vertexCount, sf::Triangles, states);

        // vertices = m_tilemap.animatedBuildings.vertices.data();
        // states.texture = m_tilemap.animatedBuildings.texture;
        // vertexCount = m_tilemap.staticBuildings.vertices.size();
        // target.draw(vertices, vertexCount, sf::Triangles, states);
    }
}