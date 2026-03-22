
#include "game/DuneII.hpp"
#include "game/scenes/mission/Mission.hpp"


#define CAMERA_VELOCITY 600
#define SCREEN_MARGIN 150


Mission::Mission(DuneII* game) noexcept:
    Scene(game, Scene::MISSION),
    m_tilemap(game, m_registry)
    // m_hud(engine, m_tilemap)
{

}

Mission::~Mission() = default;


bool Mission::load(std::string_view data) noexcept
{
    if(m_isLoaded)
        return true;

    // if(!m_hud.init())
    //     return false;

    if(m_mapLoader.loadFromFile(FileProvider::findPathToFile(std::string(data))))
    {
        if(!m_tilemap.createFromLoader(m_mapLoader))
            return false;
    }

    createSystems();

    return m_isLoaded;
}


void Mission::update(sf::Time dt) noexcept
{
    for(auto system : m_systems)
        system(this, dt);
}


void Mission::resize(sf::Vector2u size) noexcept
{
    // m_hud.resize(width, height);
}


void Mission::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_tilemap, states);
}


void Mission::createSystems() noexcept
{
//  Viewport Controller
    // m_systems.emplace_back([](Mission* mission, float dt)
    // {
    //     if(mission->m_hud.isMenuShown())
    //         return;

    //     const auto game     = mission->m_engine;
    //     const auto cursor   = game->getCursorPosition();
    //     const auto viewSize = game->getWindowsSize();
    //     const auto mapSize  = glms_ivec2_mul(mission->m_mapLoader.getMapSize(), mission->m_mapLoader.getTileSize());

    //     const bool is_near_the_left_edge   = (cursor.x > 0 && cursor.x < SCREEN_MARGIN);
    //     const bool is_near_the_top_edge    = (cursor.y > 0 && cursor.y < SCREEN_MARGIN);
    //     const bool is_near_the_right_edge  = (cursor.x > (viewSize.x - SCREEN_MARGIN) && cursor.x < viewSize.x);
    //     const bool is_near_the_bottom_edge = (cursor.y > (viewSize.y - SCREEN_MARGIN) && cursor.y < viewSize.y);

    //     const float velocity = dt * CAMERA_VELOCITY;
    //     vec2s scenePosition = mission->m_tilemap.getPosition();

    //     if(is_near_the_left_edge)
    //         scenePosition.x += velocity;
        
    //     if(is_near_the_top_edge)
    //         scenePosition.y += velocity;
        
    //     if(is_near_the_right_edge)
    //         scenePosition.x -= velocity;
        
    //     if(is_near_the_bottom_edge)
    //         scenePosition.y -= velocity;

    //     if(scenePosition.x > 0)                        scenePosition.x = 0;
    //     if(scenePosition.y > 0)                        scenePosition.y = 0;
    //     if(scenePosition.x < (viewSize.x - mapSize.x)) scenePosition.x = viewSize.x - mapSize.x;
    //     if(scenePosition.y < (viewSize.y - mapSize.y)) scenePosition.y = viewSize.y - mapSize.y;

    //     mission->m_tilemap.setPosition(scenePosition);
    // });

//  HUD Controller
    // m_systems.emplace_back([](Mission* mission, float dt)
    // {
    //     const Engine* engine = mission->m_engine;

    //     const bool isMouseButtonLeftPressed  = engine->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
    //     const bool isMouseButtonRightPressed = engine->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);

    //     if(isMouseButtonLeftPressed)
    //         mission->m_hud.runSelection();

    //     if(isMouseButtonRightPressed)
    //         mission->m_hud.cancelSelection();
 
    //     mission->m_hud.update(dt);

    //     if(mission->m_hud.isMenuShown())
    //     {
    //         if(engine->isKeyPressed(GLFW_KEY_SPACE))
    //             mission->m_hud.hideMenu();
    //     }
    // });

    m_isLoaded = true;
}