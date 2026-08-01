#include "scenes/Scene.hpp"
#include "scenes/SceneQueue.hpp"



SceneQueue::SceneQueue(Scene** scene) noexcept:
    m_current(scene)
{

}


SceneQueue::~SceneQueue() = default;


void SceneQueue::push(std::unique_ptr<Scene>& scene) noexcept
{
    if (!scene)
        return;

    if (m_scenes.size() < 2)
    { 
        m_scenes.push_back(std::move(scene));
        (*m_current) = m_scenes.back().get();

        return;
    }

    if ((*m_current) == m_scenes.back().get())
    {
        m_scenes.pop_front();
        m_scenes.push_back(std::move(scene));
    }

    (*m_current) = m_scenes.back().get();
}


void SceneQueue::pop(Scene* scene) noexcept
{
    if (m_scenes.size() > 1)
    {
        if (scene == m_scenes.back().get())
        {
            (*m_current) = m_scenes.front().get();
        }
        else if (scene == m_scenes.front().get())
        {
            m_scenes.pop_back();
        }
    }
}