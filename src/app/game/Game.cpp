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


uint32_t Game::getShaderProgram(const std::string& name) const noexcept
{
    if(auto it = shaderPrograms.find(name); it != shaderPrograms.end())
        return it->second.getHandle();

    size_t index = 0;

    if(const auto shaderPaths = FileProvider::findShaders(name); !shaderPaths.empty())
    {
        if(shaderPaths.size() < 2)
            return 0;

        std::vector<Shader> shaders(shaderPaths.size());

        for(const auto& filePath : shaderPaths)
        {
            if(filePath.extension().string() == ".vert")
            {
                if(shaders[index].loadFromFile(filePath, GL_VERTEX_SHADER))
                    ++index;
            }

            if(filePath.extension().string() == ".frag")
            {
                if(shaders[index].loadFromFile(filePath, GL_FRAGMENT_SHADER))
                    ++index;
            }

            if(filePath.extension().string() == ".geom")
            {
                if(shaders[index].loadFromFile(filePath, GL_GEOMETRY_SHADER))
                    ++index;
            }
        }

        if(index > 1)
        {
            if(ShaderProgram program; program.link(shaders))
            {
                auto it = shaderPrograms.emplace(name, std::move(program));

                if(it.second)
                    return it.first->second.getHandle();
            }
        }
    }

    return 0;
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