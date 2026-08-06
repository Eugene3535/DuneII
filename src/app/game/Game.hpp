#pragma once

#include "common/info/GameInfo.hpp"
#include "graphics/resources/GlResourceManager.hpp"
#include "app/window/WindowData.hpp"
#include "scenes/Scene.hpp"
#include "scenes/SceneManager.hpp"


struct Game final
{
    enum State : uint32_t
    {
        MAIN_MENU,
        PICK_HOUSE,
        MISSION
    };

    WindowData windowData;
    GlResourceManager glResources;
    SceneManager* sceneManager;

    uint32_t frameCounter = 0;

    GameInfo gameInfo;
};