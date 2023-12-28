template<class T>
T* SceneNode::pushScene() noexcept
{
    return dynamic_cast<T*>(m_scenes.emplace_back(std::make_unique<T>(this)).get());
}

template<class T>
bool SceneNode::setScene() noexcept
{
    for(const auto& scene : m_scenes)
        if(auto found = dynamic_cast<T*>(scene.get()); found != nullptr)
        {
            if(found->isLoaded())
            {
                m_nextScene = found;

                return true;
            }
            else
            {
                if(found->load())
                {
                    m_nextScene = found;

                    return true;
                }
                else
                {
                    removeScene(found);

                    return false;
                }
            }
        }

    if(m_rootScene)
        return m_rootScene->setScene<T>();

    if(auto newScene = pushScene<T>(); newScene->load())
    {
        m_nextScene = newScene;

        return true;
    }
    else
    {
        removeScene(newScene);
    }

    return false;
}