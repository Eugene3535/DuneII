#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "files/FileProvider.hpp"
#include "application/game/Game.hpp"
#include "scenes/mission/Mission.hpp"


#define CAMERA_VELOCITY 600
#define SCREEN_MARGIN 150



Mission::Mission(Game* game) noexcept:
    Scene(game, Scene::MISSION),
    m_tilemap(game, m_registry),
    m_menu(game, m_tilemap),
    m_hud(game, m_tilemap, m_menu)
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
    Scene::resize(width, height);
    m_hud.resize(width, height);
}


void Mission::createSystems() noexcept
{
//  Viewport
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        if (mission->m_menu.isShown())
            return;

        const auto game     = mission->m_game;
        const auto cursor   = mission->m_cursor;
        const auto viewSize = mission->m_size;
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
        const int button = mission->m_mouse.button;
        const int action = mission->m_mouse.action;
        const bool isPressed = (action != GLFW_RELEASE);

        const bool isMouseButtonLeftPressed  = ( (button == GLFW_MOUSE_BUTTON_LEFT) && isPressed );
        const bool isMouseButtonRightPressed = ( (button == GLFW_MOUSE_BUTTON_RIGHT) && isPressed );

        if (isMouseButtonLeftPressed)
            mission->m_hud.runSelection();

        if (isMouseButtonRightPressed)
            mission->m_hud.cancelSelection();

        mission->m_hud.update(dt, mission->m_cursor);
    });


//  Construction menu
    m_systems.emplace_back([](Mission* mission, float dt)
    {   
        auto& menu = mission->m_menu;
        auto game = mission->m_game;

        if (!menu.isShown())
            return;

        menu.update(dt);

        const int key = mission->m_keyboard.key;
        const int action = mission->m_keyboard.action;
        const bool isPressed = (action != GLFW_RELEASE);

        if ((key == GLFW_KEY_W) && isPressed)
        {
            menu.updateSelection('W');
        }
        else if ((key == GLFW_KEY_A) && isPressed)
        {
            menu.updateSelection('A');
        }
        else if ((key == GLFW_KEY_S) && isPressed)
        {
            menu.updateSelection('S');
        }
        else if ((key == GLFW_KEY_D) && isPressed)
        {
            menu.updateSelection('D');
        }
         
        if ((key == GLFW_KEY_SPACE) && isPressed)
        {
            if (menu.getSelectedButton() == ConstructionMenu::ButtonType::Exit)
            {
                menu.hide();

                return;
            }

            const auto selectedPreview = menu.getSelectedPreview();

            if ((selectedPreview != EntityIcon::INVALID) && (selectedPreview != EntityIcon::Empty_Cell))
            {
                auto entity = mission->m_hud.getLastSelectedEntity();
                assert(entity != entt::null);

                auto component = mission->m_registry.try_get<StructureInfo>(entity);
                assert(component);

                component->icon = selectedPreview;
                component->duration = 10; // 10 seconds for example
                component->progress = 100;
                component->isUnderConstruction = true;

                mission->m_hud.forceUpdateConstructionIcon(selectedPreview);

                menu.hide();
            }
        }
    });

//  Under construction
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        auto view = mission->m_registry.view<StructureInfo>();

        view.each([mission, dt](StructureInfo& component) 
        {
            if (component.isUnderConstruction)
            {
                component.progress -= component.duration * dt;

                if (component.progress < 1.f)
                    component.isUnderConstruction = false;
            }
        });
    });

    m_isLoaded = true;
}
