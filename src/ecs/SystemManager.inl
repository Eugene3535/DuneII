template<class T>
constexpr void checkSystemType()
{
    static_assert(std::is_base_of_v<System<T>, T>, "A system of type T must inherit System<T>");
}

template<class T, class... Args>
T* SystemManager::add(entt::registry& registry, Args&&... args) noexcept
{
    checkSystemType<T>();

    if(auto found = m_randomAccessSystems.find(T::Type); found != m_randomAccessSystems.end())
        return dynamic_cast<T*>(found->second);

    if(m_offset + sizeof(T) >= m_rawMemory.size())
        return nullptr;

    auto system = new(m_rawMemory.data() + m_offset)T(registry, std::forward<Args>(args)...);
    m_offset += sizeof(T);

    m_randomAccessSystems.emplace(T::Type, system);
    m_sequentialAccessSystems.push_back(system);

    return system;
}

template<class T>
T* SystemManager::get() noexcept
{
    if(auto found = m_randomAccessSystems.find(T::Type); found != m_randomAccessSystems.end())
        return dynamic_cast<T*>(found->second);

    return nullptr;
}