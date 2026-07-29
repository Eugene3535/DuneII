#include <cassert>

#include "scenes/Scene.hpp"
#include "scenes/SceneQueue.hpp"



SceneQueue::SceneQueue() noexcept:
    m_current(nullptr)
{

}


SceneQueue::~SceneQueue() = default;


void SceneQueue::push(std::shared_ptr<Scene> scene) noexcept
{
    assert(scene);
    assert(m_scenes.size() < 3);

    if (m_scenes.size() < 2)
    { 
        m_scenes.push_back(scene);
        m_current = scene.get();

        return;
    }

    if (m_current == m_scenes.back().get())
    {
        m_scenes.pop_front();
        m_scenes.push_back(scene);
    }

    m_current = m_scenes.back().get();
}


void SceneQueue::pop(Scene* scene) noexcept
{
    assert(m_scenes.size() < 3);

    if (m_scenes.empty())
        return;

    if (m_scenes.size() > 1)
    {
        if (scene == m_scenes.back().get())
        {
            m_current = m_scenes.front().get();
        }
        else if (scene == m_scenes.front().get())
        {
            m_scenes.pop_back();
            m_current = m_scenes.front().get();
        }
    }
    else
    {
        m_scenes.pop_back();
        m_current = nullptr;
    }
}


Scene* SceneQueue::get() const noexcept
{
    return m_current;
}