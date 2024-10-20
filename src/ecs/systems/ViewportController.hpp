#ifndef VIEWPORT_CONTROLLER_HPP
#define VIEWPORT_CONTROLLER_HPP

#include <SFML/Graphics/Rect.hpp>

#include "ecs/systems/base/System.hpp"

namespace sf
{
    class RenderWindow;
    class View;
}

class ViewportController : public System<ViewportController>
{
public:
    ViewportController(entt::registry& registry, class sf::RenderWindow& rw, class sf::View& view, const class TileMap& map) noexcept;
    ~ViewportController();

    void execute(sf::Time dt) noexcept override;

    const sf::IntRect& getViewport() const noexcept; 

private:
    class sf::RenderWindow& m_window;
    class sf::View& m_view;
    sf::IntRect m_viewport;
    sf::Vector2i m_mapSize;
    sf::Vector2i m_viewPosition;
};

#endif // !VIEWPORT_CONTROLLER_HPP