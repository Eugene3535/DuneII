#pragma once

#include <list>
#include <memory>


class SceneQueue
{
public:
    SceneQueue() noexcept;
    ~SceneQueue();

    void push(std::shared_ptr<class Scene> scene) noexcept;
    void pop(class Scene* scene) noexcept;

    class Scene* get() const noexcept;

private:
    std::list<std::shared_ptr<class Scene>> m_scenes;
    class Scene* m_current;
};