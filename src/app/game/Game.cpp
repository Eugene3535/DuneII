#include <string_view>

#include <glad/glad.h>

#include "files/FileProvider.hpp"
#include "files/Shader.hpp"
#include "scenes/intro/TitleScreen.hpp"
#include "scenes/pick_house/PickHouse.hpp"
#include "scenes/mission/Mission.hpp"
#include "app/game/Game.hpp"



bool Game::initialize() noexcept
{
    auto titleScreen = std::make_shared<TitleScreen>(this);

    if (!titleScreen->load({}))
        return false;

    scenes.push(titleScreen);

    updateData();

    return true;
}


void Game::update(float dt) noexcept
{
    if (auto currentScene = scenes.get(); currentScene)
        currentScene->update(dt);
}


void Game::draw() noexcept
{
    glClear(GL_COLOR_BUFFER_BIT);

    auto projection = windowData.view->getProjectionMatrix();

    if (auto currentScene = scenes.get(); currentScene)
        currentScene->draw(projection);
}


void Game::updateData() noexcept
{
    if (auto currentScene = scenes.get(); currentScene)
    {
        windowData.scene = currentScene;
        const auto size = windowData.view->getSize();
        currentScene->resize(size.x, size.y);
    }
}