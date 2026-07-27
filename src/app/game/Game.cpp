#include <glad/glad.h>

#include "files/FileProvider.hpp"
#include "files/Shader.hpp"
#include "scenes/intro/TitleScreen.hpp"
#include "scenes/pick_house/PickHouse.hpp"
#include "scenes/mission/Mission.hpp"
#include "app/game/Game.hpp"



Game::Game() noexcept:
    frameCounter(0),
    nextSceneType(Scene::UNSELECTED)
{

}


bool Game::initialize() noexcept
{
    if (currentScene)
        return true;

    if (currentScene = load<TitleScreen>({}); !currentScene)
        return false;

    updateData();

    return true;
}


void Game::update(float dt) noexcept
{
    if (!currentScene)
        return;

    currentScene->update(dt);

    if (nextSceneType != Scene::UNSELECTED) [[unlikely]]
    {
        switch (nextSceneType)
        {
            case Scene::Type::MAIN_MENU:
            {
                if (auto titleScene = load<TitleScreen>({}))
                    currentScene = titleScene;
            }
            break;

            case Scene::Type::PICK_HOUSE:
            {
                if (auto pickHouseScene = load<PickHouse>({}))
                    currentScene = pickHouseScene;
            }
            break;

            case Scene::Type::MISSION:
            {
                if (auto missionScene = load<Mission>("Atreides-8.tmx"))
                    currentScene = missionScene;
            }
            break;

            default:
                break;
        }

        updateData();

        nextSceneType = Scene::UNSELECTED;
    }
}


void Game::draw() noexcept
{
    glClear(GL_COLOR_BUFFER_BIT);

    auto projection = windowData.view->getProjectionMatrix();

    if (currentScene)
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
    if (currentScene)
    {
        windowData.scene = currentScene.get();
        const auto size = windowData.view->getSize();
        currentScene->resize(size.x, size.y);
    }
}