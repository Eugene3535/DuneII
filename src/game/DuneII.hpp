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
#include "animation/AnimationManager.hpp"


class DuneII final
{
public:
    enum GameScene: uint32_t
    {
        NONE,
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

//  Check if the scene requesting the change has sufficient rights
//  (example: you can only start a new mission from the main menu or the previous mission)
    void notifyChangeScene(const class Scene* requester, GameScene requested_scene) noexcept;
    std::pair<GameScene, bool> isSceneNeedToBeChanged() const noexcept;
    void resetSceneChange() noexcept;

    sf::RenderWindow window;
    sf::View         viewport;
    sf::FloatRect    visible_area;
    sf::Clock        clock;

    AnimationManager animationManager;
    
private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> m_scenes;
};

#endif // !DUNEII_HPP