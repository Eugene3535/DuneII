#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "resources/files/FileProvider.hpp"
#include "game/scenes/mission/ECS/Components.hpp"
#include "game/Engine.hpp"
#include "game/scenes/mission/Mission.hpp"


#define CAMERA_VELOCITY 600
#define SCREEN_MARGIN 150



Mission::Mission(Engine* engine) noexcept:
    Scene(engine, Scene::MISSION),
    m_tilemap(engine, m_registry),
    m_menu(engine, m_tilemap),
    m_hud(engine, m_tilemap, m_menu)
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

    m_menu.init(m_hud.getTexture());

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
}


void Mission::draw(const mat4s& projection) noexcept
{
    m_tilemap.draw(projection);

    if (!m_menu.isShown())
        m_hud.draw(projection);

    if (m_hud.isEntitySelected() || m_menu.isShown())
        m_menu.draw(projection);
}


void Mission::resize(int width, int height) noexcept
{
    m_hud.resize(width, height);
}


void Mission::createSystems() noexcept
{
//  Viewport
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        if (mission->m_menu.isShown())
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

        if (isNearTheLeftEdge)
            scenePosition.x += velocity;

        if (isNearTheTopEdge)
            scenePosition.y += velocity;

        if (isNearTheRightEdge)
            scenePosition.x -= velocity;

        if (isNearTheBottomEdge)
            scenePosition.y -= velocity;

        if (scenePosition.x > 0)                        scenePosition.x = 0;
        if (scenePosition.y > 0)                        scenePosition.y = 0;
        if (scenePosition.x < (viewSize.x - mapSize.x)) scenePosition.x = viewSize.x - mapSize.x;
        if (scenePosition.y < (viewSize.y - mapSize.y)) scenePosition.y = viewSize.y - mapSize.y;

        mission->m_tilemap.setPosition(scenePosition);
    });


//  HUD
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        const Engine* engine = mission->m_engine;

        const bool isMouseButtonLeftPressed  = engine->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        const bool isMouseButtonRightPressed = engine->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);

        if (isMouseButtonLeftPressed)
            mission->m_hud.runSelection();

        if (isMouseButtonRightPressed)
            mission->m_hud.cancelSelection();

        mission->m_hud.update(dt);
    });


//  Construction menu
    m_systems.emplace_back([](Mission* mission, float dt)
    {   
        auto& menu = mission->m_menu;
        auto& engine = mission->m_engine;

        if (!menu.isShown())
            return;

        menu.update(dt);

        if (engine->isKeyPressed(GLFW_KEY_W))
        {
            menu.updateSelection('W');
        }
        else if (engine->isKeyPressed(GLFW_KEY_A))
        {
            menu.updateSelection('A');
        }
        else if (engine->isKeyPressed(GLFW_KEY_S))
        {
            menu.updateSelection('S');
        }
        else if (engine->isKeyPressed(GLFW_KEY_D))
        {
            menu.updateSelection('D');
        }
         
        if (engine->isKeyPressed(GLFW_KEY_SPACE))
        {
            if (menu.getSelectedButton() == ConstructionMenu::ButtonType::Exit)
            {
                menu.hide();

                return;
            }

            const auto selectedPreview = menu.getSelectedPreview();

            if ((selectedPreview != EntityPreview::Icon::INVALID) && (selectedPreview != EntityPreview::Icon::Empty_Cell))
            {
                auto entity = mission->m_hud.getLastSelectedEntity();
                assert(entity != entt::null);

                auto component = mission->m_registry.try_get<Component::Construction>(entity);
                assert(component);

                component->duration = 10; // 10 seconds for example
                component->progress = 0;
                component->underConstruction = true;

                // TODO: setup progress for selected building

                menu.hide();
            }
        }
    });

    //  Under construction
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        auto view = mission->m_registry.view<Component::Construction>();

        view.each([dt](Component::Construction& component) 
        {
            if (component.underConstruction)
            {
                component.progress += component.duration * dt;

                if (component.progress > 99.f)
                    component.underConstruction = false;
            }
        });
    });

    m_isLoaded = true;
}
