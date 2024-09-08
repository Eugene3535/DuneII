#include "ecs/common/EntityContainer.hpp"

BEGIN_NAMESPACE_ECS

const EntityData::ComponentIdContainer& EntityData::getComponents() const noexcept
{
    return m_componentIds;
}

const EntityData::EntitySetTypeContainer& EntityData::getEntitySets() const noexcept
{
    return m_entitySetTypes;
}

void EntityData::addEntitySet(EntitySetType type) noexcept
{
    m_entitySetTypes.insert(type);
}

void EntityData::removeEntitySet(EntitySetType type) noexcept
{
    m_entitySetTypes.erase(type);
}

END_NAMESPACE_ECS