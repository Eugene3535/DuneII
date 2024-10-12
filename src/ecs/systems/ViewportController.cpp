#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include "scenes/mission/tilemap/TileMap.hpp"
#include "ecs/systems/ViewportController.hpp"

#define CAMERA_VELOCITY 600
#define SCREEN_MARGIN 150

ViewportController::ViewportController(entt::registry& registry, sf::RenderWindow& rw, sf::View& view, const TileMap& map) noexcept :
    System(registry),
    m_window(rw),
    m_view(view)
{
    m_mapSize = map.getMapSizeInPixels();
}

ViewportController::~ViewportController() = default;

void ViewportController::execute(sf::Time dt) noexcept
{
    auto seconds = dt.asSeconds();
    float camera_velocity = seconds * CAMERA_VELOCITY;

    sf::Vector2i mouse_position  = sf::Mouse::getPosition(m_window);
    sf::Vector2i cursor_position = static_cast<sf::Vector2i>(m_window.mapPixelToCoords(mouse_position));
    const sf::Vector2i view_size = static_cast<sf::Vector2i>(m_view.getSize());

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
    if(m_viewPosition.x + view_size.x > m_mapSize.x) m_viewPosition.x = m_mapSize.x - view_size.x;
    if(m_viewPosition.y + view_size.y > m_mapSize.y) m_viewPosition.y = m_mapSize.y - view_size.y;

    m_view.setCenter(static_cast<sf::Vector2f>(m_viewPosition + sf::Vector2i(view_size.x >> 1, view_size.y >> 1)));

    m_viewport = sf::IntRect(m_viewPosition.x, m_viewPosition.y, static_cast<int>(view_size.x), static_cast<int>(view_size.y));
}

const sf::IntRect& ViewportController::getViewport() const noexcept
{
    return m_viewport;
}