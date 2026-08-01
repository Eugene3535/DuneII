#pragma once

#include <string>

#include "common/info/GameInfo.hpp"
#include "graphics/resources/GlResourceManager.hpp"
#include "app/window/WindowData.hpp"
#include "scenes/Scene.hpp"
#include "scenes/SceneQueue.hpp"


struct Game final
{
    bool initialize()     noexcept;
    void update(float dt) noexcept;
    void draw()           noexcept;


    void updateData() noexcept;

    GlResourceManager glResources;
    WindowData windowData;

    uint32_t frameCounter = 0;

    SceneQueue scenes;
    GameInfo gameInfo;
};