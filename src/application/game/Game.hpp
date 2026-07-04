#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <cglm/struct/vec2.h>

#include "graphics/shaders/ShaderProgram.hpp"
#include "scenes/Scene.hpp"
#include "common/info/GameInfo.hpp"


class Game final
{
public:
    Game(struct WindowData& data) noexcept;

    bool initialize()     noexcept;
    void update(float dt) noexcept;
    void draw()           noexcept;

    void switchScene(const Scene* requester, Scene::Type nextScene) noexcept;
    void updateUniformBuffer(const mat4s& modelViewProjection) const noexcept;

    uint32_t      getShaderProgram(const std::string& name) const noexcept;

    const GameInfo* getInfo() const noexcept;
    
private:
    void updateData() noexcept;

    template<class T>
    std::shared_ptr<T> load(std::string_view info) noexcept;

    WindowData& m_windowData;

    std::unordered_map<Scene::Type, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene>                                  m_currentScene;
    Scene::Type                                             m_nextSceneType;
    bool                                                    m_isSceneNeedToBeChanged;

    mutable std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;

    GameInfo m_gameInfo;
};

#include "application/game/Game.inl"