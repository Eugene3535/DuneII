#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "common/Defines.hpp"
#include "common/FileProvider.hpp"
#include "game/Game.hpp"
#include "ecs/systems/AnimationController.hpp"
#include "ecs/systems/ViewportController.hpp"
#include "scenes/mission/Mission.hpp"

Mission::Mission(Game& game) noexcept:
    Scene(game),
    m_tilemap(m_registry, m_animationManager)
{

}

Mission::~Mission() = default;

bool Mission::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    if(!loadAnimations()) return false;
    if(!m_tilemap.loadFromFile(FileProvider::findPathToFile(info))) return false;
    if(!m_systems.addSystem<AnimationController>(m_registry)) return false;
    if(!m_systems.addSystem<ViewportController>(m_registry, m_game.window, m_game.viewport, m_tilemap)) return false;
    
    if(auto theme = Assets->getResource<sf::Music>("08 - Command Post.flac"); theme != nullptr)
    {
        theme->setLoop(true);
        theme->play();
    }

    m_isLoaded = true;

    return m_isLoaded;
}

void Mission::update(sf::Time dt) noexcept
{
    if(m_isLoaded)
    {
        m_systems.update(dt);

        sf::IntRect viewport = m_systems.getSystem<ViewportController>()->getViewport();

        m_sprites.clear();

        auto structure_view = m_registry.view<sf::Sprite, sf::IntRect>();

        for (auto [entity, sprite, bounds] : structure_view.each())
        {
            if(viewport.intersects(bounds))
                m_sprites.push_back(&sprite);
        }

        auto anim_view = m_registry.view<sf::IntRect, Animation>();

        for (auto [entity, bounds, animation] : anim_view.each())
        {
            if(viewport.intersects(bounds))
                m_sprites.push_back(&animation);
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

bool Mission::loadAnimations() noexcept
{
    if(auto flag_texture = Assets->getResource<sf::Texture>("Flags.png"); flag_texture != nullptr)
    {
        AnimationData flagData;

        flagData.name = "HarkonnenFlag";
        flagData.layout = AnimationData::LINEAR;
        flagData.texture = flag_texture;
        flagData.startFrame = { 0, 0, 14, 14 };
        flagData.duration = 8;
        flagData.isLooped = true;
        flagData.delay = sf::seconds(0.5f);
        m_animationManager.createAnimation(flagData);

        flagData.name = "OrdosFlag";
        flagData.startFrame = { 0, 14, 14, 14 };
        m_animationManager.createAnimation(flagData);

        flagData.name = "AtreidesFlag";
        flagData.startFrame = { 0, 28, 14, 14 };
        m_animationManager.createAnimation(flagData);

        return true;
    }

    return false;
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