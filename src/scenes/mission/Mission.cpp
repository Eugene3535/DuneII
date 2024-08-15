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
        m_isLoaded = m_builder.init(m_tilemap);
        m_buildings = m_tilemap.getAllBuildings();
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
        
    auto& view            = m_game.viewport;
    const auto view_size  = view.getSize();
    const auto map_width  = m_tilemap.mapSizeInPixels.x;
    const auto map_height = m_tilemap.mapSizeInPixels.y;
    
    if(m_viewPosition.x < 0) m_viewPosition.x = 0;
    if(m_viewPosition.y < 0) m_viewPosition.y = 0;
    if(m_viewPosition.x + view_size.x > map_width)  m_viewPosition.x = map_width - view_size.x;
    if(m_viewPosition.y + view_size.y > map_height) m_viewPosition.y = map_height - view_size.y;

    view.setCenter(m_viewPosition + (view_size * 0.5f));

    sf::IntRect viewport = sf::IntRect(m_viewPosition.x, m_viewPosition.y, view_size.x, view_size.y);

    m_drawables.clear();

    for(auto b: m_buildings)
    {
        if(viewport.intersects(b->getBounds()))
        {
            m_drawables.push_back(b);
        }     
    }  

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
        target.draw(m_tilemap);

        for(auto drawable: m_drawables)
        {
            target.draw(*drawable, states);   
        }    
    }
}