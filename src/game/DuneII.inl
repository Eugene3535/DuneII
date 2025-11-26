

template<class T>
std::shared_ptr<T> DuneII::load(std::string_view info) noexcept
{
    static_assert(std::is_base_of_v<Scene, T>, "A class of type T must inherit base class Scene");

    if (auto it = m_scenes.find(typeid(T)); it != m_scenes.end())
        return std::static_pointer_cast<T>(it->second);

    auto scene = std::make_shared<T>(this);

    if (scene->load(info))
    {
        m_scenes[typeid(T)] = scene;

        return scene;
    }

    return nullptr;
}