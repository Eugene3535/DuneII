#ifndef DUNEII_HPP
#define DUNEII_HPP

#include <vector>
#include <memory>
#include <typeindex>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "scenes/Scene.hpp"
#include "animation/AnimationManager.hpp"


class DuneII final
{
public:
    enum GameScene: uint32_t
    {
        MAIN_MENU,
        MISSION
    };


    DuneII() noexcept;
    ~DuneII();

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

    void changeScene(const class Scene* requester, GameScene nextScene) noexcept;

    sf::RenderWindow window;
    sf::View         viewport;
    sf::FloatRect    visible_area;
    sf::Clock        clock;

    AnimationManager animationManager;

    GameScene next_scene { MAIN_MENU };

    bool sceneNeedToBeChanged { false };
    
private:
    std::shared_ptr<void> m_state;
    std::unordered_map<std::type_index, std::shared_ptr<void>> m_scenes;
};

#endif // !DUNEII_HPP