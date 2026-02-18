#include <cstring>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "cglm/struct/affine-mat.h"

#include "resources/files/FileProvider.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/Engine.hpp"
#include "game/scenes/mission/Mission.hpp"


#define CAMERA_VELOCITY 600
#define SCREEN_MARGIN 150


Mission::Mission(Engine* engine) noexcept:
    Scene(engine, Scene::MISSION),
    m_tilemap(m_registry, engine),
    m_hud(engine, m_tilemap)
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
    if (m_isLoaded)
        for(auto system : m_systems)
            system(this, dt);
}


void Mission::draw() noexcept
{
    auto& camera = m_engine->camera;

    alignas(16) mat4s uniformMatrix = camera.getModelViewProjectionMatrix();
    alignas(16) mat4s modelView     = m_tilemap.getMatrix();
    alignas(16) mat4s result        = glms_mul(uniformMatrix, modelView);

    camera.updateUniformBuffer(result.raw);

    m_tilemap.draw();

//  HUD
    if(!m_hud.isMenuShown())
    {
        if(m_hud.isSelectionEnabled())
            m_hud.drawSelection();
        
        modelView = m_hud.getCursorTransform().getMatrix();
        result = glms_mul(uniformMatrix, modelView);
        camera.updateUniformBuffer(result.raw);
        m_hud.drawCursor();
    }
    else
    {
        modelView = m_hud.getMenuTransform().getMatrix();
        result = glms_mul(uniformMatrix, modelView);
        camera.updateUniformBuffer(result.raw);
        m_hud.drawMenu();
    }
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
        if(mission->m_hud.isMenuShown())
            return;

        const auto game     = mission->m_engine;
        const auto cursor   = game->getCursorPosition();
        const auto viewSize = game->getWindowsSize();
        const auto mapSize  = glms_ivec2_mul(mission->m_mapLoader.getMapSize(), mission->m_mapLoader.getTileSize());

        const bool is_near_the_left_edge   = (cursor.x > 0 && cursor.x < SCREEN_MARGIN);
        const bool is_near_the_top_edge    = (cursor.y > 0 && cursor.y < SCREEN_MARGIN);
        const bool is_near_the_right_edge  = (cursor.x > (viewSize.x - SCREEN_MARGIN) && cursor.x < viewSize.x);
        const bool is_near_the_bottom_edge = (cursor.y > (viewSize.y - SCREEN_MARGIN) && cursor.y < viewSize.y);

        const float velocity = dt * CAMERA_VELOCITY;
        vec2s scenePosition = mission->m_tilemap.getPosition();

        if(is_near_the_left_edge)
            scenePosition.x += velocity;
        
        if(is_near_the_top_edge)
            scenePosition.y += velocity;
        
        if(is_near_the_right_edge)
            scenePosition.x -= velocity;
        
        if(is_near_the_bottom_edge)
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

        if(isMouseButtonLeftPressed)
            mission->m_hud.runSelection();

        if(isMouseButtonRightPressed)
            mission->m_hud.cancelSelection();
 
        mission->m_hud.update(dt);

        if(mission->m_hud.isMenuShown())
        {
            if(engine->isKeyPressed(GLFW_KEY_SPACE))
                mission->m_hud.hideMenu();
        }
    });

    m_isLoaded = true;
}