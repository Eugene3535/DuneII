#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

class SceneManager
{
public:
    SceneManager() noexcept;

    template<class T>
    static T* push() noexcept;

    template<class T>
    static T* get() noexcept;

    static void pop(const class Scene* scene) noexcept;

private:
    std::vector<std::unique_ptr<class Scene>> m_scenes;

private:
    static SceneManager* m_instance;
};

#include "managers/SceneManager.inl"

#endif // !SCENE_MANAGER_HPP