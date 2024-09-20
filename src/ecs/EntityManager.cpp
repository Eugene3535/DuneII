#include "ecs/EntityManager.hpp"

BEGIN_NAMESPACE_ECS

EntityManager::EntityManager() noexcept
{
    auto component_count = BaseComponent::getComponentCount();

    // Component containers
    m_componentContainers.resize(component_count);

    for (size_t type = 0; type < m_componentContainers.size(); ++type)
        m_componentContainers[type] = BaseComponent::createComponentContainer(type);
        
    // Entity sets
    m_componentToEntitySets.resize(component_count);
    m_entitySets.resize(BaseEntitySet::getEntitySetCount());

    for (size_t type = 0; type < m_entitySets.size(); ++type)
        m_entitySets[type] = BaseEntitySet::createEntitySet(type, m_entities, m_componentContainers, m_componentToEntitySets);
}

void EntityManager::reserve(size_t size) noexcept
{
    m_entities.reserve(size);
}

// Entities
bool EntityManager::hasEntity(entity_t entity) const noexcept
{
    return m_entities.has(entity);
}

entity_t EntityManager::createEntity() noexcept
{
    return m_entities.emplace().first;
}

void EntityManager::removeEntity(entity_t entity) noexcept
{
    const auto& entityData = m_entities[entity];

    // Remove components
    for (auto& [componentType, componentId] : entityData.getComponents())
        m_componentContainers[componentType]->remove(componentId);

    // Send message to entity sets
    for (auto entitySetType : entityData.getEntitySets())
        m_entitySets[entitySetType]->onEntityRemoved(entity);
        
    // Remove entity
    m_entities.erase(entity);
}

void EntityManager::visitEntity(entity_t entity, const Visitor& visitor) noexcept
{
    for (const auto& [componentType, componentId] : m_entities[entity].getComponents())
        visitor.handle(componentType, m_componentContainers[componentType]->get(componentId));
}

END_NAMESPACE_ECS