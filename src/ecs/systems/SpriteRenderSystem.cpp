#include "ecs/components/Bounds.hpp"
#include "ecs/systems/SpriteRenderSystem.hpp"

BEGIN_NAMESPACE_ECS

SpriteRenderSystem::SpriteRenderSystem(ecs::EntityManager& entityManager, sf::RenderWindow& rw) noexcept :
    System(entityManager),
    m_window(rw)
{
    
}

SpriteRenderSystem::~SpriteRenderSystem() = default;

void SpriteRenderSystem::execute(sf::Time dt) noexcept
{
    m_sprites.clear();

    for (auto [entity, components] : m_entityManager.getEntitySet<ecs::Bounds, ecs::Sprite>())
    {
        auto [bounds, sprite] = components;

        if(m_viewport.intersects(bounds))
            m_sprites.push_back(&sprite);
    }

    for(auto sprite : m_sprites)
        m_window.draw(*sprite);
}

void SpriteRenderSystem::setViewport(const sf::IntRect& viewport) noexcept
{
    m_viewport = viewport;
}

END_NAMESPACE_ECS