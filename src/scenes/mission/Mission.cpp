#include "common/Defines.hpp"
#include "common/FileProvider.hpp"
#include "game/Game.hpp"
#include "ecs/systems/MovementController.hpp"
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

    m_systems.addSystem<MovementController>(m_registry);

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
        float camera_velocity = seconds * CAMERA_VELOCITY;

        auto& render_target = m_game.window;
        sf::Vector2i mouse_position = sf::Mouse::getPosition(render_target);
        sf::Vector2i cursor_position = static_cast<sf::Vector2i>(render_target.mapPixelToCoords(mouse_position));

        auto& view                   = m_game.viewport;
        const sf::Vector2i view_size = static_cast<sf::Vector2i>(view.getSize());
        const sf::Vector2i map_size  = m_tilemap.getMapSizeInPixels();

        bool is_near_the_left_edge   = (mouse_position.x > 0 && mouse_position.x < SCREEN_MARGIN);
        bool is_near_the_top_edge    = (mouse_position.y > 0 && mouse_position.y < SCREEN_MARGIN);
        bool is_near_the_right_edge  = (mouse_position.x > (view_size.x - SCREEN_MARGIN) && mouse_position.x < view_size.x);
        bool is_near_the_bottom_edge = (mouse_position.y > (view_size.y - SCREEN_MARGIN) && mouse_position.y < view_size.y);

        if(is_near_the_left_edge)
            m_viewPosition.x -= camera_velocity;
        
        if(is_near_the_top_edge)
            m_viewPosition.y -= camera_velocity;
        
        if(is_near_the_right_edge)
            m_viewPosition.x += camera_velocity;
        
        if(is_near_the_bottom_edge)
            m_viewPosition.y += camera_velocity;            
     
        if(m_viewPosition.x < 0)                        m_viewPosition.x = 0;
        if(m_viewPosition.y < 0)                        m_viewPosition.y = 0;
        if(m_viewPosition.x + view_size.x > map_size.x) m_viewPosition.x = map_size.x - view_size.x;
        if(m_viewPosition.y + view_size.y > map_size.y) m_viewPosition.y = map_size.y - view_size.y;

        view.setCenter(static_cast<sf::Vector2f>(m_viewPosition + sf::Vector2i(view_size.x >> 1, view_size.y >> 1)));

        sf::IntRect viewport = sf::IntRect(m_viewPosition.x, m_viewPosition.y, static_cast<int>(view_size.x), static_cast<int>(view_size.y));

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