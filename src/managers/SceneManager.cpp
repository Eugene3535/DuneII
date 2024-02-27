#include "scenes/Scene.hpp"
#include "managers/SceneManager.hpp"

SceneManager* SceneManager::m_instance;

SceneManager::SceneManager() noexcept
{
    if(m_instance == nullptr)  
        m_instance = this;
}

void SceneManager::pop(const Scene* scene) noexcept
{
    if(m_instance)
    {
        for(auto& scenePtr : m_instance->m_scenes)
            if(scene == scenePtr.get())
            {
                if(m_instance->m_scenes.size() > 1)
                    scenePtr.swap(m_instance->m_scenes.back());

                m_instance->m_scenes.pop_back();

                return;
            }
    }
}