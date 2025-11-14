#ifndef DUNE_II_HPP
#define DUNE_II_HPP

#include <string_view>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include <glm/vec2.hpp>
#include <entt/entity/registry.hpp>

#include "resources/ogl/holder/GlResourceManager.hpp"
#include "game/scenes/Scene.hpp"


class DuneII final
{
    friend class Application;

public:
    DuneII() noexcept;

    void switchScene(const Scene* requester, Scene::Type nextScene) noexcept;

    const glm::ivec2& getWindowSize() const noexcept;
    const glm::vec2& getCursorPosition() const noexcept;

//  Resources
    GlResourceManager glResources;

//  ECS
    entt::registry registry;
    entt::entity camera;
    
private:
    template<class T>
    T* load(std::string_view info) noexcept;

    glm::ivec2 m_windowSize;
    glm::vec2 m_cursorPosition;

    std::unordered_map<std::type_index, std::shared_ptr<void>> m_scenes;
    Scene* m_currentScene;
    Scene::Type m_nextSceneType;
    bool m_isSceneNeedToBeChanged;
};

#include "game/DuneII.inl"

#endif // !DUNE_II_HPP