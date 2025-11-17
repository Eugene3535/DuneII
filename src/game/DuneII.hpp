#ifndef DUNE_II_HPP
#define DUNE_II_HPP

#include <string_view>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include <entt/entity/registry.hpp>

#include "graphics/OrthogonalCamera.hpp"
#include "resources/ogl/holder/GlResourceManager.hpp"
#include "game/scenes/Scene.hpp"


class DuneII final
{
    friend class Application;

public:
    DuneII() noexcept;

    bool init() noexcept;
    void update(float dt) noexcept;
    void draw() noexcept;

    void updateUniformBuffer(mat4 modelViewProjection) noexcept;

    void click(int button) noexcept;
    void setCursorPosition(float x, float y) noexcept;
    void resize(int width, int height) noexcept;

    void switchScene(const Scene* requester, Scene::Type nextScene) noexcept;

//  Resources
    GlResourceManager glResources;

//  ECS
    entt::registry registry;
    OrthogonalCamera camera;
    
private:
    template<class T>
    T* load(std::string_view info) noexcept;

    std::unordered_map<std::type_index, std::shared_ptr<void>> m_scenes;
    Scene* m_currentScene;
    Scene::Type m_nextSceneType;
    bool m_isSceneNeedToBeChanged;

    GLBuffer m_uniformBuffer;
};

#include "game/DuneII.inl"

#endif // !DUNE_II_HPP