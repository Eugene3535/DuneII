#pragma once

#include "common/info/GameInfo.hpp"
#include "graphics/resources/GlResourceManager.hpp"
#include "app/window/WindowData.hpp"
#include "scenes/Scene.hpp"
#include "scenes/SceneManager.hpp"


struct Game final
{
    WindowData windowData;
    GlResourceManager glResources;
    SceneManager* sceneManager;

    uint32_t frameCounter = 0;

    GameInfo gameInfo;
};