// Components
template<class T, class ...Args>
T& EntityManager::addComponent(entity_t entity, Args&&... args) noexcept
{
    checkComponentType<T>();

    auto [componentId, component] = getComponentSparseSet<T>().emplace(std::forward<Args>(args)...);
    m_entities[entity].addComponent<T>(componentId);

    // Send message to entity sets
    for (auto entitySet : m_componentToEntitySets[T::Type])
        entitySet->onEntityUpdated(entity);

    // Return the created component
    return component;
}

template<class T>
void EntityManager::removeComponent(entity_t entity) noexcept
{
    checkComponentType<T>();

    // Remove component from entity and component container
    getComponentSparseSet<T>().erase(m_entities[entity].removeComponent<T>());

    // Send message to entity sets
    for (auto entitySet : m_componentToEntitySets[T::Type])
        entitySet->onEntityUpdated(entity);
}

template<class T>
bool EntityManager::hasComponent(entity_t entity) const noexcept
{
    checkComponentType<T>();

    return m_entities[entity].hasComponent<T>();
}

template<class ...Ts>
bool EntityManager::hasComponents(entity_t entity) const noexcept
{
    checkComponentTypes<Ts...>();

    return m_entities[entity].hasComponents<Ts...>();
}

template<class T>
T& EntityManager::getComponent(entity_t entity) noexcept
{
    checkComponentType<T>();

    return getComponentSparseSet<T>()[m_entities[entity].getComponent<T>()];
}

template<class T>
const T& EntityManager::getComponent(entity_t entity) const noexcept
{
    checkComponentType<T>();

    return getComponentSparseSet<T>().get(m_entities[entity].getComponent<T>());
}

template<class ...Ts>
std::tuple<Ts&...> EntityManager::getComponents(entity_t entity) noexcept
{
    checkComponentTypes<Ts...>();
    auto& entityData = m_entities[entity];

    return std::tie(getComponentSparseSet<Ts>()[entityData.getComponent<Ts>()]...);
}

template<class ...Ts>
std::tuple<const Ts&...> EntityManager::getComponents(entity_t entity) const noexcept
{
    checkComponentTypes<Ts...>();
    auto& entityData = m_entities[entity];

    return std::tie(std::as_const(getComponentSparseSet<Ts>().get(entityData.getComponent<Ts>()))...);
}

// Entity sets
template<class ...Ts>
EntitySet<Ts...>& EntityManager::getEntitySet() noexcept
{
    checkComponentTypes<Ts...>();

    return *static_cast<EntitySet<Ts...>*>(m_entitySets[EntitySet<Ts...>::Type].get());
}

template<class ...Ts>
const EntitySet<Ts...>& EntityManager::getEntitySet() const noexcept
{
    checkComponentTypes<Ts...>();
    
    return *static_cast<EntitySet<Ts...>*>(m_entitySets[EntitySet<Ts...>::Type].get());
}

template<class T>
ComponentSparseSet<T>& EntityManager::getComponentSparseSet() noexcept
{
    return static_cast<ComponentContainer<T>*>(m_componentContainers[T::Type].get())->components;
}

template<class T>
const ComponentSparseSet<T>& EntityManager::getComponentSparseSet() const noexcept
{
    return static_cast<const ComponentContainer<T>*>(m_componentContainers[T::Type].get())->components;
}