#include "ecs/common/EntitySet.hpp"

BEGIN_NAMESPACE_ECS

BaseEntitySet::BaseEntitySet() noexcept
{

}

size_t BaseEntitySet::getEntitySetCount() noexcept
{
    return s_factories.size();
}

std::unique_ptr<BaseEntitySet> BaseEntitySet::createEntitySet(
    size_t type,
    EntityContainer& entities,
    const std::vector<std::unique_ptr<BaseComponentContainer>>& componentContainers,
    std::vector<std::vector<BaseEntitySet*>>& componentToEntitySets) noexcept
{
    return s_factories[type](entities, componentContainers, componentToEntitySets);
}

bool BaseEntitySet::hasEntity(Entity entity) const noexcept
{
    return m_entityToIndex.find(entity) != std::end(m_entityToIndex);
}

void BaseEntitySet::onEntityUpdated(Entity entity) noexcept
{
    auto satisfied = satisfyRequirements(entity);
    auto managed = hasEntity(entity);

    if (satisfied && !managed)
        addEntity(entity);
    else if (!satisfied && managed)
        removeEntity(entity, true);
}

void BaseEntitySet::onEntityRemoved(Entity entity) noexcept
{
    removeEntity(entity, false);
}

// default implementations (avoid abstract methods)
bool BaseEntitySet::satisfyRequirements(Entity entity) noexcept 
{ 
    return false; 
}

void BaseEntitySet::addEntity(Entity entity) noexcept
{

}

void BaseEntitySet::removeEntity(Entity entity, bool updateEntity) noexcept 
{

}

END_NAMESPACE_ECS