#ifndef ENTITY_SET_HPP
#define ENTITY_SET_HPP

#include <functional>
#include <memory>

#include "ecs/common/ComponentContainer.hpp"
#include "ecs/common/EntitySetIterator.hpp"
#include "ecs/common/EntityContainer.hpp"

BEGIN_NAMESPACE_ECS

class BaseEntitySet
{
public:
    BaseEntitySet() noexcept;
    virtual ~BaseEntitySet() = default;

    static size_t getEntitySetCount() noexcept;

    static std::unique_ptr<BaseEntitySet> createEntitySet(size_t type,
                                                          EntityContainer& entities,
                                                          const std::vector<std::unique_ptr<BaseComponentContainer>>& componentContainers,
                                                          std::vector<std::vector<BaseEntitySet*>>& componentToEntitySets) noexcept;

    bool hasEntity(entity_t entity) const noexcept;
    void onEntityUpdated(entity_t entity) noexcept;
    void onEntityRemoved(entity_t entity) noexcept;

protected:
    virtual bool satisfyRequirements(entity_t entity)             noexcept;
    virtual void addEntity(entity_t entity)                       noexcept;
    virtual void removeEntity(entity_t entity, bool updateEntity) noexcept;

    std::unordered_map<entity_t, size_t> m_entityToIndex;

    template<class ...Ts>
    static entity_set_t generateEntitySetType() noexcept;

private:
    using EntitySetFactory = std::unique_ptr<BaseEntitySet>(*)(EntityContainer&,
                                                               const std::vector<std::unique_ptr<BaseComponentContainer>>&,
                                                               std::vector<std::vector<BaseEntitySet*>>&);

    static std::vector<EntitySetFactory> s_factories;
};

inline std::vector<BaseEntitySet::EntitySetFactory> BaseEntitySet::s_factories;

template<class ...Ts>
class EntitySet : public BaseEntitySet
{
    using ValueType           = std::pair<entity_t, std::array<component_id_t, sizeof...(Ts)>>;
    using UIterator           = class std::vector<ValueType>::iterator;       // Underlying iterator
    using UConstIterator      = class std::vector<ValueType>::const_iterator; // Underlying const iterator
    using ComponentContainers = std::tuple<ComponentSparseSet<Ts>&...>;

public:
    using Iterator              = EntitySetIterator<UIterator, Ts...>;
    using ConstIterator         = EntitySetIterator<UConstIterator, const Ts...>;
    using ListenerId            = uint32_t;
    using EntityAddedListener   = std::function<void(entity_t)>;
    using EntityRemovedListener = std::function<void(entity_t)>;

    static const entity_set_t Type;

    EntitySet(EntityContainer& entities, const ComponentContainers& componentContainers) noexcept;

    size_t        getSize() const noexcept;

    Iterator      begin()       noexcept;
    ConstIterator begin() const noexcept;
    Iterator      end()         noexcept;
    ConstIterator end()   const noexcept;

    // Listeners
    ListenerId addEntityAddedListener(EntityAddedListener listener)     noexcept;
    void       removeEntityAddedListener(ListenerId id)                 noexcept;
    ListenerId addEntityRemovedListener(EntityRemovedListener listener) noexcept;
    void       removeEntityRemovedListener(ListenerId id)               noexcept;

protected:
    bool satisfyRequirements(entity_t entity)             noexcept override;
    void addEntity(entity_t entity)                       noexcept override;
    void removeEntity(entity_t entity, bool updateEntity) noexcept override;

private:
    std::vector<ValueType>                       m_managedEntities;
    EntityContainer&                             m_entities;
    ComponentContainers                          m_componentContainers;
    SparseSet<ListenerId, EntityAddedListener>   m_entityAddedListeners;
    SparseSet<ListenerId, EntityRemovedListener> m_entityRemovedListeners;
};

#include "ecs/common/EntitySet.inl"

template<class ...Ts>
const entity_set_t EntitySet<Ts...>::Type = BaseEntitySet::generateEntitySetType<Ts...>();

END_NAMESPACE_ECS

#endif // !ENTITY_SET_HPP
