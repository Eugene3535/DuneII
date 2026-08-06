#include "app/window/WindowData.hpp"
#include "scenes/Scene.hpp"
#include "scenes/SceneManager.hpp"



SceneManager::SceneManager(Scene** scene, WindowData* data) noexcept:
    m_current(scene),
    m_windowData(data)
{

}


SceneManager::~SceneManager() = default;


void SceneManager::push(std::unique_ptr<Scene>& scene) noexcept
{
    if (!scene)
        return;

    if (m_scenes.size() < 2)
    {
        m_scenes.push_back(std::move(scene));
        (*m_current) = m_scenes.back().get();
        updateScene();

        return;
    }

    if ((*m_current) == m_scenes.front().get())
    {
        if (m_scenes.back() != scene)
        {
            m_scenes.pop_back();
            m_scenes.push_back(std::move(scene));
        }
    }
    else
    {
        m_scenes.pop_front();
        m_scenes.push_back(std::move(scene));
    }

    (*m_current) = m_scenes.back().get();
    updateScene();
}


void SceneManager::pop() noexcept
{
    if (m_scenes.size() > 1)
    {
        if ((*m_current) == m_scenes.back().get())
        {
            (*m_current) = m_scenes.front().get();
        }
        else if ((*m_current) == m_scenes.front().get())
        {
            m_scenes.pop_back();
        }

        updateScene();
    }
}


void SceneManager::updateScene() noexcept
{
    m_windowData->scene = *m_current;
    const auto size = m_windowData->view->getSize();
    m_windowData->scene->resize(size.x, size.y);
}