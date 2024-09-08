#ifndef ENTITY_CONTAINER_HPP
#define ENTITY_CONTAINER_HPP

#include <unordered_map>
#include <unordered_set>

#include "ecs/common/SparseSet.hpp"

BEGIN_NAMESPACE_ECS
class EntityData
{
    using ComponentIdContainer   = std::unordered_map<ComponentType, ComponentId>;
    using EntitySetTypeContainer = std::unordered_set<EntitySetType>;

public:
//  Components
    template<class T>
    void addComponent(ComponentId id) noexcept;

    template<class T>
    bool hasComponent() const noexcept;

    template<class ...Ts>
    bool hasComponents() const noexcept;

    template<class T>
    ComponentId getComponent() const noexcept;

    template<class T>
    ComponentId removeComponent() noexcept;

    const ComponentIdContainer& getComponents() const noexcept;

//  Types
    const EntitySetTypeContainer& getEntitySets() const noexcept;
    void addEntitySet(EntitySetType type) noexcept;
    void removeEntitySet(EntitySetType type) noexcept;

private:
    template<class T>
    ComponentIdContainer::const_iterator findComponent() const noexcept;

private:
    ComponentIdContainer   m_componentIds;
    EntitySetTypeContainer m_entitySetTypes;
};

#include "ecs/common/EntityContainer.inl"

using EntityContainer = SparseSet<Entity, EntityData>;

END_NAMESPACE_ECS

#endif // !ENTITY_CONTAINER_HPP