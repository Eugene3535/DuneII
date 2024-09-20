#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "ecs/common/EntitySet.hpp"
#include "ecs/common/Visitor.hpp"

BEGIN_NAMESPACE_ECS

class EntityManager
{
public:
    static constexpr uint32_t UndefinedEntity = UINT32_MAX;

public:
    EntityManager() noexcept;

    void reserve(size_t size) noexcept;
    
//  Entities
    bool   hasEntity(entity_t entity)                     const noexcept;
    entity_t createEntity()                                     noexcept;
    void   removeEntity(entity_t entity)                        noexcept;
    void   visitEntity(entity_t entity, const Visitor& visitor) noexcept;

//  Components
    template<class T, class ...Args>
    T& addComponent(entity_t entity, Args&&... args) noexcept;

    template<class T>
    void removeComponent(entity_t entity) noexcept;
    
    template<class T>
    bool hasComponent(entity_t entity) const noexcept;

    template<class ...Ts>
    bool hasComponents(entity_t entity) const noexcept;

    template<class T>
    T& getComponent(entity_t entity) noexcept;

    template<class T>
    const T& getComponent(entity_t entity) const noexcept;

    template<class ...Ts>
    std::tuple<Ts&...> getComponents(entity_t entity) noexcept;

    template<class ...Ts>
    std::tuple<const Ts&...> getComponents(entity_t entity) const noexcept;

//  Entity sets
    template<class ...Ts>
    EntitySet<Ts...>& getEntitySet() noexcept;

    template<class ...Ts>
    const EntitySet<Ts...>& getEntitySet() const noexcept;

private:
    template<class T>
    ComponentSparseSet<T>& getComponentSparseSet() noexcept;

    template<class T>
    const ComponentSparseSet<T>& getComponentSparseSet() const noexcept;

private:
    std::vector<std::unique_ptr<BaseComponentContainer>> m_componentContainers;
    EntityContainer                                      m_entities;
    std::vector<std::unique_ptr<BaseEntitySet>>          m_entitySets;
    std::vector<std::vector<BaseEntitySet*>>             m_componentToEntitySets;
};

#include "ecs/EntityManager.inl"

END_NAMESPACE_ECS

#endif // !ENTITY_MANAGER_HPP
