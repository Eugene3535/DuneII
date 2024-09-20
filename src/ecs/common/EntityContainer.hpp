#ifndef ENTITY_CONTAINER_HPP
#define ENTITY_CONTAINER_HPP

#include <unordered_map>
#include <unordered_set>

#include "ecs/common/SparseSet.hpp"

BEGIN_NAMESPACE_ECS
class EntityData
{
    using ComponentIdContainer   = std::unordered_map<component_t, component_id_t>;
    using EntitySetTypeContainer = std::unordered_set<entity_set_t>;

public:
//  Components
    template<class T>
    void addComponent(component_id_t id) noexcept;

    template<class T>
    bool hasComponent() const noexcept;

    template<class ...Ts>
    bool hasComponents() const noexcept;

    template<class T>
    component_id_t getComponent() const noexcept;

    template<class T>
    component_id_t removeComponent() noexcept;

    const ComponentIdContainer& getComponents() const noexcept;

//  Types
    const EntitySetTypeContainer& getEntitySets() const noexcept;
    void addEntitySet(entity_set_t type) noexcept;
    void removeEntitySet(entity_set_t type) noexcept;

private:
    template<class T>
    ComponentIdContainer::const_iterator findComponent() const noexcept;

private:
    ComponentIdContainer   m_componentIds;
    EntitySetTypeContainer m_entitySetTypes;
};

#include "ecs/common/EntityContainer.inl"

using EntityContainer = SparseSet<entity_t, EntityData>;

END_NAMESPACE_ECS

#endif // !ENTITY_CONTAINER_HPP