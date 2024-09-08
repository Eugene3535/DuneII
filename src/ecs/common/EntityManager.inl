// Components
template<class T>
bool EntityManager::hasComponent(Entity entity) const noexcept
{
    checkComponentType<T>();

    return m_entities[entity].hasComponent<T>();
}

template<class ...Ts>
bool EntityManager::hasComponents(Entity entity) const noexcept
{
    checkComponentTypes<Ts...>();

    return m_entities[entity].hasComponents<Ts...>();
}

template<class T>
T& EntityManager::getComponent(Entity entity) noexcept
{
    checkComponentType<T>();

    return getComponentSparseSet<T>()[m_entities[entity].getComponent<T>()];
}

template<class T>
const T& EntityManager::getComponent(Entity entity) const noexcept
{
    checkComponentType<T>();

    return getComponentSparseSet<T>().get(m_entities[entity].getComponent<T>());
}

template<class ...Ts>
std::tuple<Ts&...> EntityManager::getComponents(Entity entity) noexcept
{
    checkComponentTypes<Ts...>();
    auto& entityData = m_entities[entity];

    return std::tie(getComponentSparseSet<Ts>()[entityData.getComponent<Ts>()]...);
}

template<class ...Ts>
std::tuple<const Ts&...> EntityManager::getComponents(Entity entity) const noexcept
{
    checkComponentTypes<Ts...>();
    auto& entityData = m_entities[entity];

    return std::tie(std::as_const(getComponentSparseSet<Ts>().get(entityData.getComponent<Ts>()))...);
}

template<class T, class ...Args>
T& EntityManager::addComponent(Entity entity, Args&&... args) noexcept
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
void EntityManager::removeComponent(Entity entity) noexcept
{
    checkComponentType<T>();

    // Remove component from entity and component container
    getComponentSparseSet<T>().erase(m_entities[entity].removeComponent<T>());

    // Send message to entity sets
    for (auto entitySet : m_componentToEntitySets[T::Type])
        entitySet->onEntityUpdated(entity);
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