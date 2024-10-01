#include "common/FileProvider.hpp"
#include "game/Game.hpp"
#include "ecs/systems/MoveSystem.hpp"
#include "scenes/mission/Mission.hpp"

Mission::Mission(Game& game) noexcept:
    Scene(game),
    m_tilemap(m_registry)
{

}

Mission::~Mission() = default;

bool Mission::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    m_systems.add<MoveSystem>(m_registry);

    if(m_isLoaded = m_tilemap.loadFromFile(FileProvider().findPathToFile(info)); m_isLoaded)
    {       
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

        m_sprites.clear();

        auto ecs_view = m_registry.view<sf::Sprite, sf::IntRect>();

        for (auto [entity, sprite, bounds] : ecs_view.each())
        {
            if(viewport.intersects(bounds))
                m_sprites.push_back(&sprite);
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

        m_systems.update(dt);
    }
}

void Mission::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isLoaded)
    {
        target.draw(m_tilemap, states);

        for(auto sprite : m_sprites)
            target.draw(*sprite, states);
    }
}