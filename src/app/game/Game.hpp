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


    uint32_t getShaderProgram(const std::string& name) const noexcept;
    void updateData() noexcept;

    template<class T>
    std::shared_ptr<T> load(std::string_view info) noexcept;

    WindowData windowData;

    uint32_t frameCounter;

    std::unordered_map<Scene::Type, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene>                                  currentScene;
    Scene::Type                                             nextSceneType;

    mutable std::unordered_map<std::string, ShaderProgram> shaderPrograms;

    GameInfo gameInfo;
};

#include "app/game/Game.inl"