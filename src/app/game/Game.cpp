#include <string_view>

#include <glad/glad.h>

#include "files/FileProvider.hpp"
#include "files/Shader.hpp"
#include "scenes/intro/TitleScreen.hpp"
#include "scenes/pick_house/PickHouse.hpp"
#include "scenes/mission/Mission.hpp"
#include "app/game/Game.hpp"


void Game::updateData() noexcept
{
    if(!scene)
        return;
        
    windowData.scene = scene;
    const auto size = windowData.view->getSize();
    scene->resize(size.x, size.y);
}