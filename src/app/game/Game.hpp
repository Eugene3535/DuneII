#pragma once

#include <utility>

#include <entt/entity/registry.hpp>

#include "common/info/GameInfo.hpp"
#include "common/tasks/TaskManager.hpp"
#include "common/tasks/TaskData.hpp"
#include "common/tasks/Task.hpp"
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

    entt::registry registry;

    TaskManager taskManager;
    std::vector<std::pair<void*, uint32_t(*)(void*, float)>> tasks;
};