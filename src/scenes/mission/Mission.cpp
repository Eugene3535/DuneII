#include "common/FileProvider.hpp"
#include "game/Game.hpp"
#include "scenes/mission/Mission.hpp"

Mission::Mission(Game& game) noexcept:
    Scene(game)
{

}

Mission::~Mission()
{
}

bool Mission::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    if(m_isLoaded = m_tilemap.loadFromFile(FileProvider().findPathToFile(info)); m_isLoaded)
    {
        m_buildings = m_tilemap.getAllBuildings();
        m_isLoaded = m_builder.init(m_tilemap);
    }

    return m_isLoaded;
}

void Mission::update(sf::Time dt) noexcept
{
    if(!m_isLoaded)
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_viewPosition.x -= 10;
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_viewPosition.x += 10;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_viewPosition.y -= 10;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_viewPosition.y += 10;

    auto& viewport  = m_game.viewport;
    auto view_size  = viewport.getSize();
    auto map_width  = m_tilemap.mapSizeInPixels.x;
    auto map_height = m_tilemap.mapSizeInPixels.y;
    
    if(m_viewPosition.x < 0) m_viewPosition.x = 0;
    if(m_viewPosition.y < 0) m_viewPosition.y = 0;
    if(m_viewPosition.x + view_size.x > map_width)  m_viewPosition.x = map_width - view_size.x;
    if(m_viewPosition.y + view_size.y > map_height) m_viewPosition.y = map_height - view_size.y;

    viewport.setCenter(m_viewPosition + (view_size * 0.5f));

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

        const auto& view = target.getView();

        sf::Vector2i center(view.getCenter());
        sf::IntRect  viewport = target.getViewport(view);
        viewport.left = center.x - (viewport.width >> 1);
        viewport.top  = center.y - (viewport.height >> 1);

        for(auto& b: m_buildings)
        {
            if(viewport.intersects(b->getBounds()))
            {
                target.draw(*b, states);
            }     
        }        
    }
}