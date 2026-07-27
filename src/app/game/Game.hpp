#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <cglm/struct/vec2.h>

#include "common/info/GameInfo.hpp"
#include "graphics/shaders/ShaderProgram.hpp"
#include "app/window/WindowData.hpp"
#include "scenes/Scene.hpp"


struct Game final
{
    Game() noexcept;

    bool initialize()     noexcept;
    void update(float dt) noexcept;
    void draw()           noexcept;

    void switchScene(const Scene* requester, Scene::Type nextScene) noexcept;

    uint32_t getShaderProgram(const std::string& name) const noexcept;

    const GameInfo* getInfo() const noexcept;

    void updateData() noexcept;

    template<class T>
    std::shared_ptr<T> load(std::string_view info) noexcept;

    WindowData windowData;

    uint32_t frameCounter;

    std::unordered_map<Scene::Type, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene>                                  m_currentScene;
    Scene::Type                                             m_nextSceneType;
    bool                                                    m_isSceneNeedToBeChanged;

    mutable std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;

    GameInfo m_gameInfo;
};

#include "app/game/Game.inl"