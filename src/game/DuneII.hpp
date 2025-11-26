#ifndef DUNE_II_HPP
#define DUNE_II_HPP

#include <string_view>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include <cglm/struct/vec2.h>

#include "resources/files/FileProvider.hpp"
#include "resources/ogl/holder/GlResourceManager.hpp"
#include "graphics/camera/OrthogonalCamera.hpp"
#include "game/scenes/Scene.hpp"


class DuneII final
{
    friend class Application;

public:
    DuneII() noexcept;

    bool init(struct GLFWwindow* window) noexcept;
    void update(float dt) noexcept;
    void draw() noexcept;

    void updateUniformBuffer(mat4 modelViewProjection) noexcept;

    void press(int key) noexcept;
    void click(int button) noexcept;
    void setCursorPosition(float x, float y) noexcept;
    void resize(int width, int height) noexcept;

    void switchScene(const Scene* requester, Scene::Type nextScene) noexcept;

    ivec2s getWindowsSize() const noexcept;

//  Resources
    FileProvider fileProvider;
    GlResourceManager glResources;

    OrthogonalCamera camera;
    
private:
    template<class T>
    std::shared_ptr<T> load(std::string_view info) noexcept;

    struct GLFWwindow* m_window;

    std::unordered_map<std::type_index, std::shared_ptr<void>> m_scenes;
    std::shared_ptr<Scene> m_currentScene;
    Scene::Type m_nextSceneType;
    bool m_isSceneNeedToBeChanged;

    GLBuffer m_uniformBuffer;
};

#include "game/DuneII.inl"

#endif // !DUNE_II_HPP