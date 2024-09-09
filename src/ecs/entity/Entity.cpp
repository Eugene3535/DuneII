#include <cassert>

#include "ecs/entity/Entity.hpp"

BEGIN_NAMESPACE_ECS

EntityManager* Entity::s_entityManager;

Entity::Entity() noexcept:
    m_id(EntityManager::UndefinedEntity)
{
    s_entityManager = EntityManager::instance();
    assert((s_entityManager != nullptr) && "First, an instance of the EntityManager must be created.");
}

bool Entity::create() noexcept
{
    if(!exist())
    {
        m_id = s_entityManager->createEntity();

        return true;
    }

    return false;
}

bool Entity::exist() const noexcept
{
    return s_entityManager->hasEntity(m_id);
}

void Entity::destroy() noexcept
{
    if(exist())
        s_entityManager->removeEntity(m_id);
}

entity_t Entity::getId() const noexcept
{
    return m_id;
}

END_NAMESPACE_ECS