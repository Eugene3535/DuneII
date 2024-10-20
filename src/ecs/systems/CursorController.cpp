#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include "ui/Cursor.hpp"
#include "scenes/mission/tilemap/TileMap.hpp"
#include "ecs/systems/CursorController.hpp"

CursorController::CursorController(entt::registry& registry, sf::RenderWindow& rw, Cursor& cursor, TileMap& map) noexcept :
    System(registry),
    m_window(rw),
    m_cursor(cursor),
    m_tilemap(map)
{
    m_window.setMouseCursorVisible(false);
}

CursorController::~CursorController()
{
    m_window.setMouseCursorVisible(true);
}

void CursorController::execute(sf::Time dt) noexcept
{
    sf::Vector2i mouse_position  = sf::Mouse::getPosition(m_window);
    auto world_position = m_window.mapPixelToCoords(mouse_position);

    m_cursor.setPosition(static_cast<sf::Vector2f>(world_position));

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        m_cursor.capture();
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        m_cursor.release();
    }
}
