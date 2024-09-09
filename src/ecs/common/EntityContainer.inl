template<class T>
void EntityData::addComponent(component_id_t id) noexcept
{
    m_componentIds[T::Type] = id;
}

template<class T>
bool EntityData::hasComponent() const noexcept
{
    return findComponent<T>() != std::end(m_componentIds);
}

template<class ...Ts>
bool EntityData::hasComponents() const noexcept
{
    return (hasComponent<Ts>() && ...);
}

template<class T>
component_id_t EntityData::getComponent() const noexcept
{
    return findComponent<T>()->second;
}

template<class T>
component_id_t EntityData::removeComponent() noexcept
{
    auto it = findComponent<T>();
    auto componentId = it->second;
    m_componentIds.erase(it);

    return componentId;
}

template<class T>
EntityData::ComponentIdContainer::const_iterator EntityData::findComponent() const noexcept
{
    return m_componentIds.find(T::Type);
}