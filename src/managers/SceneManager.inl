template<class T>
T* SceneManager::push() noexcept
{
    if(m_instance)
    {
        if(auto scene = get<T>(); scene != nullptr)
            return scene;

        auto& sceneRef = m_instance->m_scenes.emplace_back(std::make_unique<T>());

        return dynamic_cast<T*>(sceneRef.get());
    }

    return nullptr;
}

template<class T>
T* SceneManager::get() noexcept
{
    if(m_instance)
    {
        if(m_instance->m_scenes.empty())
            return nullptr;

        for(const auto& scene : m_instance->m_scenes)
            if(auto raw_ptr = dynamic_cast<T*>(scene.get()); raw_ptr != nullptr)
                return raw_ptr;
    }

    return nullptr;
}