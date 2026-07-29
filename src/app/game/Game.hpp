#pragma once

#include <string>

#include "common/info/GameInfo.hpp"
#include "graphics/shaders/ShaderProgram.hpp"
#include "app/window/WindowData.hpp"
#include "scenes/Scene.hpp"
#include "scenes/SceneQueue.hpp"


struct Game final
{
    bool initialize()     noexcept;
    void update(float dt) noexcept;
    void draw()           noexcept;


    uint32_t getShaderProgram(const std::string& name) const noexcept;
    void updateData() noexcept;

    WindowData windowData;

    uint32_t frameCounter = 0;

    SceneQueue scenes;

    mutable std::unordered_map<std::string, ShaderProgram> shaderPrograms;

    GameInfo gameInfo;
};