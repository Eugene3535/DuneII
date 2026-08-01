#pragma once

#include "common/info/GameInfo.hpp"
#include "graphics/resources/GlResourceManager.hpp"
#include "app/window/WindowData.hpp"
#include "scenes/Scene.hpp"
#include "scenes/SceneQueue.hpp"


struct Game final
{

    void updateData() noexcept;

    std::unique_ptr<SceneQueue> scenes;
    Scene* scene;

    GlResourceManager glResources;
    WindowData windowData;

    uint32_t frameCounter = 0;

    GameInfo gameInfo;
};