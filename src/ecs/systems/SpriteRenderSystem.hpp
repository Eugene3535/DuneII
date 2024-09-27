#ifndef SPRITE_RENDER_SYSTEM_HPP
#define SPRITE_RENDER_SYSTEM_HPP

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "ecs/systems/base/System.hpp"
#include "ecs/components/Sprite.hpp"

BEGIN_NAMESPACE_ECS

class SpriteRenderSystem : public System<SpriteRenderSystem>
{
public:
    SpriteRenderSystem(ecs::EntityManager& entityManager, sf::RenderWindow& rw) noexcept;
    ~SpriteRenderSystem();

    void execute(sf::Time dt) noexcept override;

    void setViewport(const sf::IntRect& viewport) noexcept;

private:
    sf::RenderWindow&         m_window;
    std::vector<ecs::Sprite*> m_sprites;
    sf::IntRect               m_viewport;
};

END_NAMESPACE_ECS

#endif // !SPRITE_RENDER_SYSTEM_HPP