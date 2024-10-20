#ifndef CURSOR_CONTROLLER_HPP
#define CURSOR_CONTROLLER_HPP

#include "ecs/systems/base/System.hpp"

namespace sf
{
    class RenderWindow;
}

class CursorController : public System<CursorController>
{
public:
    CursorController(entt::registry& registry, class sf::RenderWindow& rw, class Cursor& cursor, class TileMap& map) noexcept;
    ~CursorController();

    void execute(sf::Time dt) noexcept override;

private:
    class sf::RenderWindow& m_window;
    class Cursor& m_cursor;
    class TileMap& m_tilemap;
};

#endif // !CURSOR_CONTROLLER_HPP