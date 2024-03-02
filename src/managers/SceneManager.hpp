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
    T* push(class Game* game) noexcept
    {
        if(auto scene = get<T>(); scene != nullptr)
            return scene;

        auto& sceneRef = m_scenes.emplace_back(std::make_unique<T>(game));

        return dynamic_cast<T*>(sceneRef.get());
    }

    template<class T>
    T* get() noexcept
    {
        if(m_scenes.empty())
            return nullptr;

        for(const auto& scene : m_scenes)
            if(auto raw_ptr = dynamic_cast<T*>(scene.get()); raw_ptr != nullptr)
                return raw_ptr;

        return nullptr;
    }

    void pop(const class Scene* scene) noexcept;

private:
    std::vector<std::unique_ptr<class Scene>> m_scenes;
};

#endif // !SCENE_MANAGER_HPP