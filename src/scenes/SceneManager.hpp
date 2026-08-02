#pragma once

#include <list>
#include <memory>


class SceneManager
{
public:
    SceneManager(class Scene** scene, struct WindowData* data) noexcept;
    ~SceneManager();

    void push(std::unique_ptr<class Scene>& scene) noexcept;
    void pop(class Scene* scene) noexcept;

private:
    void updateScene() noexcept;

    class Scene** m_current;
    WindowData* m_windowData;
    std::list<std::unique_ptr<class Scene>> m_scenes;
};