#ifndef DUNE_II_HPP
#define DUNE_II_HPP

#include <string_view>
#include <unordered_map>
#include <memory>

#include <cglm/struct/vec2.h>

#include "resources/gl_interfaces/shaders/ShaderProgram.hpp"
#include "graphics/camera/OrthogonalCamera.hpp"
#include "game/scenes/Scene.hpp"


class DuneII final
{
    friend class Application;

public:
    DuneII() noexcept;

    void switchScene(const Scene* requester, Scene::Type nextScene) noexcept;

    bool isKeyPressed(int key) const noexcept;
    bool isMouseButtonPressed(int button) const noexcept;

    uint32_t getShaderProgram(const std::string& name) noexcept;
    vec2s    getCursorPosition() const noexcept;
    ivec2s   getWindowsSize() const noexcept;

    OrthogonalCamera camera;
    
private:
    bool init(struct GLFWwindow* window) noexcept;
    void update(float dt)                noexcept;
    void draw()                          noexcept;
    void resize(int width, int height)   noexcept;

    template<class T>
    std::shared_ptr<T> load(std::string_view info) noexcept;

    struct GLFWwindow* m_window;

    std::unordered_map<Scene::Type, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene> m_currentScene;
    Scene::Type m_nextSceneType;
    bool m_isSceneNeedToBeChanged;

    std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;
};

#include "game/DuneII.inl"

#endif // !DUNE_II_HPP