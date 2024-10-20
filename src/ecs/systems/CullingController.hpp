#ifndef CULLING_CONTROLLER_HPP
#define CULLING_CONTROLLER_HPP

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "ecs/systems/base/System.hpp"

class CullingController : public System<CullingController>
{
public:
    CullingController(entt::registry& registry, std::vector<sf::Sprite*>& sprites, const sf::IntRect& viewport) noexcept;
    ~CullingController();

    void execute(sf::Time dt) noexcept override;

private:
    std::vector<sf::Sprite*>& m_sprites;
    const sf::IntRect& m_viewport;
};

#endif // !CULLING_CONTROLLER_HPP