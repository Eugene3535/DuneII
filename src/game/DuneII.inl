template<class T>
bool DuneII::load(std::string_view info) noexcept
{
    static_assert(std::is_base_of_v<Scene, T>, "A class of type T must inherit base class Scene");

    auto scene = std::make_shared<T>(this);

    if (auto it = m_scenes.find(scene->getType()); it != m_scenes.end())
    {
        m_currentScene = std::static_pointer_cast<T>(it->second);

        return true;
    }

    if (scene->load(info))
    {
        m_scenes[scene->getType()] = scene;
        m_currentScene = scene;

        return true;
    }

    return false;
}