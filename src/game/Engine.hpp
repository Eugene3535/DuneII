#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <string>
#include <unordered_map>
#include <memory>

#include <cglm/struct/vec2.h>

#include "resources/gl_interfaces/shaders/ShaderProgram.hpp"
#include "graphics/camera/OrthoMatrix.hpp"
#include "game/scenes/Scene.hpp"
#include "common/info/GameInfo.hpp"


class Engine final
{
    friend class MainWindow;

public:
    Engine() noexcept;

    void switchScene(const Scene* requester, Scene::Type nextScene) noexcept;
    void updateUniformBuffer(const mat4s& modelViewProjection) const noexcept;

    bool isKeyPressed(int key) const noexcept;
    bool isMouseButtonPressed(int button) const noexcept;

    uint32_t      getShaderProgram(const std::string& name) const noexcept;
    const ivec2s& getWindowsSize()                          const noexcept;
    const vec2s&  getCursorPosition()                       const noexcept;

    const GameInfo* getInfo() const noexcept;
    
private:
    bool init(struct GLFWwindow* window) noexcept;
    void update(float dt)                noexcept;
    void draw()                          noexcept;
    void resize(int width, int height)   noexcept;

    template<class T>
    std::shared_ptr<T> load(std::string_view info) noexcept;

    struct GLFWwindow* m_window;
    ivec2s             m_windowSize;
    vec2s              m_cursorPosition;

    std::unordered_map<Scene::Type, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene>                                  m_currentScene;
    Scene::Type                                             m_nextSceneType;
    bool                                                    m_isSceneNeedToBeChanged;

    mutable std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;

    OrthoMatrix m_orthoMatrix;

    GameInfo m_gameInfo;
};

#include "game/Engine.inl"

#endif // !GAME_ENGINE_HPP