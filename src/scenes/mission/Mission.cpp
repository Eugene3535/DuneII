#include "common/FileProvider.hpp"
#include "game/Game.hpp"
#include "ecs/systems/MoveSystem.hpp"
#include "scenes/mission/Mission.hpp"

Mission::Mission(Game& game) noexcept:
    Scene(game),
    m_tilemap(m_entityManager)
{

}

Mission::~Mission() = default;

bool Mission::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    m_systems.initialize();
    m_systems.addSystem<MoveSystem>(m_entityManager);

    if(m_isLoaded = m_tilemap.loadFromFile(FileProvider().findPathToFile(info)); m_isLoaded)
    {       
        m_buildings = m_tilemap.getAllBuildings();

        if(auto theme = Assets->getResource<sf::Music>("08 - Command Post.flac"); theme != nullptr)
        {
            theme->setLoop(true);
            theme->play();
        }
    }

    return m_isLoaded;
}

void Mission::update(sf::Time dt) noexcept
{
    if(m_isLoaded)
    {
        auto seconds = dt.asSeconds();
        float movement = seconds * 600; // camera speed

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_viewPosition.x -= movement;
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_viewPosition.x += movement;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_viewPosition.y -= movement;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_viewPosition.y += movement;
            
        auto& view           = m_game.viewport;
        const auto view_size = view.getSize();
        const auto map_size  = m_tilemap.getMapSizeInPixels();
        
        if(m_viewPosition.x < 0) m_viewPosition.x = 0;
        if(m_viewPosition.y < 0) m_viewPosition.y = 0;
        if(m_viewPosition.x + view_size.x > map_size.x) m_viewPosition.x = map_size.x - view_size.x;
        if(m_viewPosition.y + view_size.y > map_size.y) m_viewPosition.y = map_size.y - view_size.y;

        view.setCenter(m_viewPosition + (view_size * 0.5f));

        sf::IntRect viewport = sf::IntRect(m_viewPosition.x, m_viewPosition.y, view_size.x, view_size.y);

        m_drawables.clear();

        for(auto b: m_buildings)
        {
            if(viewport.intersects(b->bounds))
            {
                m_drawables.push_back(b);
            }     
        }  

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            m_game.sceneNeedToBeChanged = true;
            m_game.next_scene = Game::GameScene::MAIN_MENU;

            if(auto theme = Assets->getResource<sf::Music>("08 - Command Post.flac"); theme != nullptr)
            {
                theme->stop();
            }
        }
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