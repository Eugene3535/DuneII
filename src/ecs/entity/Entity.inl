template<class T, class ...Args>
T& Entity::addComponent(Args&&... args) noexcept
{
    return s_entityManager->addComponent<T>(m_id, std::forward<Args>(args)...);
}

template<class T>
void Entity::removeComponent() noexcept
{
    s_entityManager->removeComponent<T>(m_id);
}

template<class T>
bool Entity::hasComponent() const noexcept
{
    return s_entityManager->hasComponent<T>(m_id);
}

template<class ...Ts>
bool Entity::hasComponents() const noexcept
{
    return s_entityManager->hasComponents<Ts...>(m_id);
}

template<class T>
T& Entity::getComponent() noexcept
{
    checkComponentType<T>();

    return s_entityManager->getComponent<T>(m_id);
}

template<class T>
const T& Entity::getComponent() const noexcept
{
    return s_entityManager->getComponent<T>(m_id);
}

template<class ...Ts>
std::tuple<Ts&...> Entity::getComponents() noexcept
{
    return s_entityManager->getComponents<Ts...>(m_id);
}

template<class ...Ts>
std::tuple<const Ts&...> Entity::getComponents() const noexcept
{
    return s_entityManager->getComponents<Ts...>(m_id);
}
