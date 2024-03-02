#include "scenes/Scene.hpp"
#include "managers/SceneManager.hpp"

SceneManager::SceneManager() noexcept
{

}

void SceneManager::pop(const Scene* scene) noexcept
{
    for(auto& scenePtr : m_scenes)
        if(scene == scenePtr.get())
        {
            if(m_scenes.size() > 1)
                scenePtr.swap(m_scenes.back());

            m_scenes.pop_back();

            return;
        }
}