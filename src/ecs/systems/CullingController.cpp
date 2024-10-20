#include "ecs/components/Animation.hpp"
#include "ecs/systems/CullingController.hpp"

#ifdef DEBUG
#include <cstdio>
#endif

CullingController::CullingController(entt::registry& registry, std::vector<sf::Sprite*>& sprites, const sf::IntRect& viewport) noexcept :
    System(registry),
    m_sprites(sprites),
    m_viewport(viewport)
{

}

CullingController::~CullingController() = default;

void CullingController::execute(sf::Time dt) noexcept
{
    m_sprites.clear();

    auto structure_view = m_registry.view<sf::Sprite, sf::IntRect>();

    for (auto [entity, sprite, bounds] : structure_view.each())
    {
        if(m_viewport.intersects(bounds))
            m_sprites.push_back(&sprite);
    }

    auto anim_view = m_registry.view<sf::IntRect, Animation>();

    for (auto [entity, bounds, animation] : anim_view.each())
    {
        if(m_viewport.intersects(bounds))
            m_sprites.push_back(&animation);
    }

#ifdef DEBUG
    static sf::Time timer = sf::Time::Zero;

    if(timer > sf::seconds(1.0f))
    {
        timer = sf::Time::Zero;
        printf("Number of sprites on the screen: %zu\n", m_sprites.size());
    }
    timer += dt;
#endif
}
