#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "common/Defines.hpp"
#include "common/FileProvider.hpp"
#include "assets/AssetManager.hpp"
#include "animation/AnimationData.hpp"
#include "ecs/components/Structure.hpp"
#include "scenes/mission/tilemap/Tile.hpp"
#include "game/DuneII.hpp"
#include "scenes/mission/Mission.hpp"


Mission::Mission(DuneII* game) noexcept:
    Scene(game),
    m_tilemap(m_registry, game->animationManager)
{

}


Mission::~Mission() = default;


bool Mission::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    if(!loadAnimations()) return false;
    if(!m_cursor.load(m_game->animationManager)) return false;
    if(!m_tilemap.loadFromFile(FileProvider::findPathToFile(info))) return false;

    createSystems();
    
    if(auto theme = Assets->getResource<sf::Music>(COMMAND_POST_FLAC); theme != nullptr)
    {
        theme->setLooping(true);
        theme->play();
    }

    m_isLoaded = true;

    return m_isLoaded;
}


void Mission::update(const sf::Time dt) noexcept
{
    if(m_isLoaded)
    {
        for(auto system : m_systems)
            system(this, dt);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
        {
            m_game->notifyChangeScene(this, DuneII::GameScene::MAIN_MENU);
            m_game->window.setMouseCursorVisible(true);

            if(auto theme = Assets->getResource<sf::Music>(COMMAND_POST_FLAC); theme != nullptr)
            {
                theme->stop();
            }
        }
    }
}


sf::Vector2i Mission::resize(const sf::Vector2u& size) noexcept
{
    return m_viewPosition;
}


bool Mission::loadAnimations() noexcept
{
    AnimationData flagData;

    flagData.name = "HarkonnenFlag";
    flagData.layout = AnimationData::LINEAR;
    flagData.startFrame = {{ 0, 0 }, { 14, 14 }};
    flagData.duration = 8;
    flagData.isLooped = true;
    flagData.delay = sf::seconds(0.5f);

    if(!m_game->animationManager.createAnimation(flagData)) 
        return false;

    flagData.name = "OrdosFlag";
    flagData.startFrame = {{ 0, 14 }, { 14, 14 }};

    if(!m_game->animationManager.createAnimation(flagData)) 
        return false;

    flagData.name = "AtreidesFlag";
    flagData.startFrame = {{ 0, 28 }, { 14, 14 }};

    if(!m_game->animationManager.createAnimation(flagData)) 
        return false;

    return true;
}


void Mission::createSystems() noexcept
{
//  Animations
    m_systems.emplace_back([](Mission* mission, sf::Time dt)
    {
        auto view = mission->m_registry.view<Animation, sf::Sprite>();

        for (auto [entity, animation, sprite] : view.each())
        {
            if( ! animation.isOver )
            {
                animation.timer += dt;

                if(animation.timer > animation.delay)
                {
                    animation.currentFrame++;
                    animation.timer = sf::Time::Zero;

                    if(animation.currentFrame == animation.frames.size())
                    {
                        if(animation.isLooped)
                        {
                            animation.currentFrame = 0;
                        }
                        else
                        {
                            animation.isOver = true;
                            continue;
                        }
                    }

                    sprite.setTextureRect(animation.frames[animation.currentFrame]);
                }
            }
        }
    });


//  ViewportController
    m_systems.emplace_back([](Mission* mission, sf::Time dt)
    {
        constexpr int CAMERA_VELOCITY = 600;
        constexpr int SCREEN_MARGIN = 150;

        const sf::Vector2i& m_mapSize = mission->m_tilemap.getMapSizeInPixels();
        auto& viewPosition = mission->m_viewPosition;

        auto seconds = dt.asSeconds();
        float camera_velocity = seconds * CAMERA_VELOCITY;

        sf::Vector2i mouse_position  = sf::Mouse::getPosition(mission->m_game->window);
        const sf::Vector2i view_size = static_cast<sf::Vector2i>(mission->m_game->viewport.getSize());

        bool is_near_the_left_edge   = (mouse_position.x > 0 && mouse_position.x < SCREEN_MARGIN);
        bool is_near_the_top_edge    = (mouse_position.y > 0 && mouse_position.y < SCREEN_MARGIN);
        bool is_near_the_right_edge  = (mouse_position.x > (view_size.x - SCREEN_MARGIN) && mouse_position.x < view_size.x);
        bool is_near_the_bottom_edge = (mouse_position.y > (view_size.y - SCREEN_MARGIN) && mouse_position.y < view_size.y);

        if(is_near_the_left_edge)
            viewPosition.x -= camera_velocity;
        
        if(is_near_the_top_edge)
            viewPosition.y -= camera_velocity;
        
        if(is_near_the_right_edge)
            viewPosition.x += camera_velocity;
        
        if(is_near_the_bottom_edge)
            viewPosition.y += camera_velocity;            
        
        if(viewPosition.x < 0)                         viewPosition.x = 0;
        if(viewPosition.y < 0)                         viewPosition.y = 0;
        if(viewPosition.x + view_size.x > m_mapSize.x) viewPosition.x = m_mapSize.x - view_size.x;
        if(viewPosition.y + view_size.y > m_mapSize.y) viewPosition.y = m_mapSize.y - view_size.y;

        mission->m_game->viewport.setCenter(static_cast<sf::Vector2f>(viewPosition + sf::Vector2i(view_size.x >> 1, view_size.y >> 1)));
        mission->m_viewport = sf::IntRect({viewPosition.x, viewPosition.y}, {view_size.x, view_size.y});
    });


//  CullingController
    m_systems.emplace_back([](Mission* mission, sf::Time dt)
    {
        mission->m_sprites.clear();

        auto structure_view = mission->m_registry.view<Tile, sf::IntRect>();

        for (auto [entity, tile, bounds] : structure_view.each())
        {
            if(mission->m_viewport.findIntersection(bounds))
                mission->m_sprites.push_back(&tile);
        }

        auto anim_view = mission->m_registry.view<sf::IntRect, Animation, sf::Sprite>();

        for (auto [entity, bounds, animation, sprite] : anim_view.each())
        {
            if(mission->m_viewport.findIntersection(bounds))
                mission->m_sprites.push_back(&sprite);
        }

#ifdef DEBUG
        static sf::Time timer = sf::Time::Zero;

        if(timer > sf::seconds(1.0f))
        {
            timer = sf::Time::Zero;
            printf("Number of sprites on the screen: %zu\n", mission->m_sprites.size());
        }
        timer += dt;
#endif
    });


//  CursorController
    m_systems.emplace_back([](Mission* mission, sf::Time dt)
    {
        static constexpr int32_t cooldown = 4;
        static int timer = 0;

        sf::Vector2i mouse_position = sf::Mouse::getPosition(mission->m_game->window);
        auto world_position = mission->m_game->window.mapPixelToCoords(mouse_position);

        mission->m_cursor.update(world_position, dt);

        if(timer > cooldown)
        {
            timer = 0;

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if(auto entity = mission->m_tilemap.getEntityUnderCursor(static_cast<sf::Vector2i>(world_position)); entity.has_value())
                {
                    if(auto [structure, bounds] = mission->m_registry.try_get<Structure, sf::IntRect>(entity.value()); structure != nullptr)
                    {
                        bool can_be_highlighted = 
                                    ((structure->type != StructureType::SLAB_1x1) &&
                                    ( structure->type != StructureType::SLAB_2x2) && 
                                    ( structure->type != StructureType::WALL)     && 
                                    structure->type < StructureType::MAX);

                        if(can_be_highlighted)
                        {
                            mission->m_cursor.setVertexFrame(*bounds);
                            mission->m_cursor.select();
                        }
                        else
                        {
                            mission->m_cursor.release();
                        }
                    }
                }
                else
                {
                    mission->m_cursor.release();
                }

                //m_cursor.capture(); // for units
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            {
                mission->m_cursor.release();
            }
        }

        ++timer;
    });
}


void Mission::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isLoaded)
    {
        target.draw(m_tilemap, states);

        for(auto sprite : m_sprites)
            target.draw(*sprite, states);

        target.draw(m_cursor, states);
    }
}