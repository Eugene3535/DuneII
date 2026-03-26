#ifndef DUNE_II_HPP
#define DUNE_II_HPP

#include <unordered_map>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "resources/files/assets/AssetManager.hpp"
#include "graphics/animation/AnimationManager.hpp"
#include "game/scenes/Scene.hpp"
#include "common/info/GameInfo.hpp"


class DuneII final: public sf::Drawable 
{
public:
    DuneII(sf::RenderWindow& rw) noexcept;

    bool init(/* GameState& state */) noexcept;
    void update(sf::Time dt)       noexcept;
    void resize(sf::Vector2u size) noexcept;

    void switchScene(const Scene* requester, Scene::Type nextScene) noexcept;

    const GameInfo& getInfo() const noexcept;

    sf::RenderWindow& window;

    AssetManager assets;
    AnimationManager animations;
    
private:
    template<class T>
    bool load(std::string_view data) noexcept; // TODO: std::string_view -> class GameState to save progress

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::unordered_map<Scene::Type, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene>                                  m_currentScene;
    Scene::Type                                             m_nextSceneType;
    bool                                                    m_isSceneNeedToBeChanged;

    GameInfo m_gameInfo;
};

#include <game/DuneII.inl>

#endif // !DUNE_II_HPP