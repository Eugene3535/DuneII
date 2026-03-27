
#include "game/DuneII.hpp"
#include "game/scenes/mission/Mission.hpp"


Mission::Mission(DuneII* game) noexcept:
    Scene(game, Scene::MISSION),
    m_tilemap(game, m_registry),
    m_hud(game, m_tilemap)
{

}

Mission::~Mission() = default;


bool Mission::load(std::string_view data) noexcept
{
    if(m_isLoaded)
        return true;

    if(!m_hud.load(data))
        return false;

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

    m_hud.update(dt);
}


void Mission::resize(sf::Vector2u size) noexcept
{
    Scene::resize(size);
    m_hud.resize(size);
}


void Mission::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.setView(m_view);
    target.draw(m_tilemap, states);
    target.draw(m_hud, states);
}


void Mission::createSystems() noexcept
{
//  Viewport Controller
    m_systems.emplace_back([](Mission* mission, sf::Time dt)
    {
        // if(mission->m_hud.isMenuShown())
        //     return;

        const int32_t CAMERA_VELOCITY = 600;
        const int32_t SCREEN_MARGIN = 150;

        const sf::Vector2i tileSize = mission->m_mapLoader.getTileSize();
        sf::Vector2i mapSize = mission->m_mapLoader.getMapSize();
        mapSize.x *= tileSize.x;
        mapSize.y *= tileSize.y;

        auto& viewPosition = mission->m_viewPosition;
        float cameraVelocity = dt.asSeconds() * CAMERA_VELOCITY;

        sf::Vector2i mousePosition  = sf::Mouse::getPosition(mission->m_game->window);
        const sf::Vector2i viewSize = static_cast<sf::Vector2i>(mission->m_view.getSize());

        bool isNearTheLeftEdge   = (mousePosition.x > 0 && mousePosition.x < SCREEN_MARGIN);
        bool isNearTheTopEdge    = (mousePosition.y > 0 && mousePosition.y < SCREEN_MARGIN);
        bool isNearTheRightEdge  = (mousePosition.x > (viewSize.x - SCREEN_MARGIN) && mousePosition.x < viewSize.x);
        bool isNearTheBottomEdge = (mousePosition.y > (viewSize.y - SCREEN_MARGIN) && mousePosition.y < viewSize.y);

        if(isNearTheLeftEdge)
            viewPosition.x -= cameraVelocity;
        
        if(isNearTheTopEdge)
            viewPosition.y -= cameraVelocity;
        
        if(isNearTheRightEdge)
            viewPosition.x += cameraVelocity;
        
        if(isNearTheBottomEdge)
            viewPosition.y += cameraVelocity;            
        
        if(viewPosition.x < 0)                      viewPosition.x = 0;
        if(viewPosition.y < 0)                      viewPosition.y = 0;
        if(viewPosition.x + viewSize.x > mapSize.x) viewPosition.x = mapSize.x - viewSize.x;
        if(viewPosition.y + viewSize.y > mapSize.y) viewPosition.y = mapSize.y - viewSize.y;

        mission->m_view.setCenter(static_cast<sf::Vector2f>(viewPosition + sf::Vector2i(viewSize.x >> 1, viewSize.y >> 1)));
        mission->m_viewport = sf::IntRect({viewPosition.x, viewPosition.y}, {viewSize.x, viewSize.y});
    });


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