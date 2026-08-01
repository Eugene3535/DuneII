#pragma once

#include <list>
#include <memory>


class SceneQueue
{
public:
    SceneQueue(class Scene** scene) noexcept;
    ~SceneQueue();

    void push(std::unique_ptr<class Scene>& scene) noexcept;
    void pop(class Scene* scene) noexcept;

private:
    std::list<std::unique_ptr<class Scene>> m_scenes;
    class Scene** m_current;
};