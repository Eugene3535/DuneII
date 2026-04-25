#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "resources/files/FileProvider.hpp"
#include "game/Engine.hpp"
#include "game/scenes/mission/action/ActionData.hpp"
#include "game/scenes/mission/action/Action.hpp"
#include "game/scenes/mission/Mission.hpp"


#define CAMERA_VELOCITY 600
#define SCREEN_MARGIN 150
#define ACTION_MEMORY_POOL_SIZE (1024 << 2)



Mission::Mission(Engine* engine) noexcept:
    Scene(engine, Scene::MISSION),
    m_tilemap(engine, m_registry),
    m_hud(engine, m_tilemap),
    m_actionAllocator(ACTION_MEMORY_POOL_SIZE)
{

}

Mission::~Mission()
{

}


bool Mission::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;

    if(!m_hud.init())
        return false;

    if(m_mapLoader.loadFromFile(FileProvider::findPathToFile(std::string(info))))
    {
        if(!m_tilemap.createFromLoader(m_mapLoader))
            return false;
    }

    createSystems();

    return m_isLoaded;
}


void Mission::update(float dt) noexcept
{
    for(auto system : m_systems)
        system(this, dt);

    auto quick_remove_at = [](auto& v, size_t idx)
    {    
        if (idx < v.size()) 
            v[idx] = v.back();
        
        v.pop_back();
    };
    
    for (size_t i = 0; i < m_actions.size(); ++i)
    {
        auto action = m_actions[i];
        auto data = m_actionData[i];
        size_t result = action(data, dt);

        if(result)
        {
            m_actionAllocator.release(data, result);
            quick_remove_at(m_actions, i);
            quick_remove_at(m_actionData, i);
        }
    }
}


void Mission::draw(const mat4s& projection) noexcept
{
    m_tilemap.draw(projection);
    m_hud.draw(projection);
}


void Mission::resize(int width, int height) noexcept
{
    m_hud.resize(width, height);
}


void Mission::createSystems() noexcept
{
//  Viewport Controller
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        if(mission->m_hud.getMenu().isShown())
            return;

        const auto game     = mission->m_engine;
        const auto cursor   = game->getCursorPosition();
        const auto viewSize = game->getWindowsSize();
        const auto mapSize  = glms_ivec2_mul(mission->m_mapLoader.getMapSize(), mission->m_mapLoader.getTileSize());

        const bool isNearTheLeftEdge   = (cursor.x > 0 && cursor.x < SCREEN_MARGIN);
        const bool isNearTheTopEdge    = (cursor.y > 0 && cursor.y < SCREEN_MARGIN);
        const bool isNearTheRightEdge  = (cursor.x > (viewSize.x - SCREEN_MARGIN) && cursor.x < viewSize.x);
        const bool isNearTheBottomEdge = (cursor.y > (viewSize.y - SCREEN_MARGIN) && cursor.y < viewSize.y);

        const float velocity = dt * CAMERA_VELOCITY;
        vec2s scenePosition = mission->m_tilemap.getPosition();

        if(isNearTheLeftEdge)
            scenePosition.x += velocity;

        if(isNearTheTopEdge)
            scenePosition.y += velocity;

        if(isNearTheRightEdge)
            scenePosition.x -= velocity;

        if(isNearTheBottomEdge)
            scenePosition.y -= velocity;

        if(scenePosition.x > 0)                        scenePosition.x = 0;
        if(scenePosition.y > 0)                        scenePosition.y = 0;
        if(scenePosition.x < (viewSize.x - mapSize.x)) scenePosition.x = viewSize.x - mapSize.x;
        if(scenePosition.y < (viewSize.y - mapSize.y)) scenePosition.y = viewSize.y - mapSize.y;

        mission->m_tilemap.setPosition(scenePosition);
    });

//  HUD Controller
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        const Engine* engine = mission->m_engine;

        const bool isMouseButtonLeftPressed  = engine->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        const bool isMouseButtonRightPressed = engine->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);
        const bool isSpaceKeyPressed = engine->isKeyPressed(GLFW_KEY_SPACE);

        if (isMouseButtonLeftPressed)
            mission->m_hud.runSelection();

        if (isMouseButtonRightPressed)
            mission->m_hud.cancelSelection();

        mission->m_hud.update(dt);

        auto& menu = mission->m_hud.getMenu();

        if (menu.isShown())
        {
            if (isSpaceKeyPressed)
            {
                if (menu.getSelectedButton() == ConstructionMenu::ButtonType::Exit)
                {
                    menu.hide();

                    return;
                }

                const auto selectedPreview = menu.getSelectedPreview();

                if ((selectedPreview != PreviewType::INVALID) && (selectedPreview != PreviewType::Empty_Cell))
                {
                    if (void* actionData = mission->m_actionAllocator.allocate(sizeof(Action::Construction)))
                    {
                        auto* data = static_cast<Action::Construction*>(actionData);

                        data->duration = 10; // 10 seconds for example
                        data->countdown = 100;
                        data->progress = menu.getProgress();
                        
                        mission->m_actions.push_back(Action::construct);
                        mission->m_actionData.push_back(actionData);

                        menu.hide();
                        menu.showEntityView(selectedPreview, true);
                    }
                }
            }
        }
    });

    m_isLoaded = true;
}
