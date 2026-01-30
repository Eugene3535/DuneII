

template<class T>
std::shared_ptr<T> Engine::load(std::string_view info) noexcept
{
    static_assert(std::is_base_of_v<Scene, T>, "A class of type T must inherit base class Scene");

    auto scene = std::make_shared<T>(this);

    if (auto it = m_scenes.find(scene->getType()); it != m_scenes.end())
        return std::static_pointer_cast<T>(it->second);

    if (scene->load(info))
    {
        m_scenes[scene->getType()] = scene;

        return scene;
    }

    return nullptr;
}