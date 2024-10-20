#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "common/Defines.hpp"
#include "common/FileProvider.hpp"
#include "game/Game.hpp"
#include "ecs/systems/AnimationController.hpp"
#include "ecs/systems/ViewportController.hpp"
#include "ecs/systems/CullingController.hpp"
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
    if(!m_cursor.load(m_animationManager)) return false;
    if(!m_tilemap.loadFromFile(FileProvider::findPathToFile(info))) return false;

    if(!m_systems.addSystem<AnimationController>(m_registry)) return false;

    if(auto vc = m_systems.addSystem<ViewportController>(m_registry, m_game.window, m_game.viewport, m_tilemap); vc != nullptr)
        m_systems.addSystem<CullingController>(m_registry, m_sprites, vc->getViewport());
    else 
        return false;
    
    if(auto theme = Assets->getResource<sf::Music>(COMMAND_POST_FLAC); theme != nullptr)
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

        // sf::IntRect viewport = m_systems.getSystem<ViewportController>()->getViewport();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            m_game.sceneNeedToBeChanged = true;
            m_game.next_scene = Game::GameScene::MAIN_MENU;

            if(auto theme = Assets->getResource<sf::Music>(COMMAND_POST_FLAC); theme != nullptr)
            {
                theme->stop();
            }
        }
    }
}

bool Mission::loadAnimations() noexcept
{
    if(auto flag_texture = Assets->getResource<sf::Texture>(FLAGS_PNG); flag_texture != nullptr)
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