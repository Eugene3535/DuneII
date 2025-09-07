#ifndef DUNEII_HPP
#define DUNEII_HPP

#include <vector>
#include <memory>
#include <utility>
#include <typeindex>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "scenes/Scene.hpp"
#include "assets/AssetManager.hpp"
#include "animation/AnimationManager.hpp"


class DuneII final
{
public:
    AssetManager& getAssets() noexcept;

    template<class T>
    T* load(const std::string& info) noexcept
    {
        static_assert(std::is_base_of_v<Scene, T>, "A class of type T must inherit base class Scene");

        if(auto it = m_scenes.find(typeid(T)); it != m_scenes.end())
            return static_cast<T*>(it->second.get());

        auto scene = std::make_shared<T>(this);

        if(scene->load(info))
        {
            m_scenes[typeid(T)] = scene;

            return scene.get();
        }
            
        return nullptr;
    }

    bool checkSceneRights(const Scene* requester, Scene::Type requestedType) noexcept;


    sf::RenderWindow window;
    sf::FloatRect    visible_area;
    sf::Clock        clock;

    AnimationManager animationManager;
    
private:
    AssetManager m_assets;
    std::unordered_map<std::type_index, std::shared_ptr<void>> m_scenes;
};

#endif // !DUNEII_HPP