#include <glad/glad.h>

#include "files/FileProvider.hpp"
#include "files/Shader.hpp"
#include "scenes/intro/TitleScreen.hpp"
#include "scenes/pick_house/PickHouse.hpp"
#include "scenes/mission/Mission.hpp"
#include "application/window/WindowData.hpp"
#include "application/game/Game.hpp"



Game::Game(WindowData& data) noexcept:
    m_windowData(data),
    m_nextSceneType(Scene::NONE),
    m_isSceneNeedToBeChanged(false)
{

}


bool Game::initialize() noexcept
{
    if (m_currentScene)
        return true;

    glClearColor(0.f, 0.f, 0.f, 1.f);

    if (m_currentScene = load<TitleScreen>({}); !m_currentScene)
        return false;

    updateData();

    return true;
}


void Game::update(float dt) noexcept
{
    if (!m_currentScene)
        return;

    m_currentScene->update(dt);

    if (m_isSceneNeedToBeChanged) [[unlikely]]
    {
        switch (m_nextSceneType)
        {
            case Scene::Type::MAIN_MENU:
            {
                if (auto titleScene = load<TitleScreen>({}))
                    m_currentScene = titleScene;
            }
            break;

            case Scene::Type::PICK_HOUSE:
            {
                if (auto pickHouseScene = load<PickHouse>({}))
                    m_currentScene = pickHouseScene;
            }
            break;

            case Scene::Type::MISSION:
            {
                if (auto missionScene = load<Mission>("Atreides-8.tmx"))
                    m_currentScene = missionScene;

                // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            break;

            default:
                break;
        }

        updateData();

        m_isSceneNeedToBeChanged = false;
        m_nextSceneType = Scene::NONE;
    }
}


void Game::draw() noexcept
{
    glClear(GL_COLOR_BUFFER_BIT);

    auto projection = m_windowData.viewport.getProjectionMatrix();

    if (m_currentScene)
        m_currentScene->draw(projection);
}


void Game::switchScene(const Scene* requester, Scene::Type nextScene) noexcept
{
    if (!requester)
        return;

    switch (nextScene)
    {
        case Scene::MAIN_MENU:
            if (requester->getType() == Scene::MISSION)
            {
                m_nextSceneType = nextScene;
                m_isSceneNeedToBeChanged = true;
            }
        break;

        case Scene::PICK_HOUSE:
            if(requester->getType() == Scene::MAIN_MENU)
            {
                m_nextSceneType = nextScene;
                m_isSceneNeedToBeChanged = true;
            }
        break;

        case Scene::MISSION:
            if(requester->getType() == Scene::PICK_HOUSE)
            {
                m_nextSceneType = nextScene;
                m_isSceneNeedToBeChanged = true;
            }
        break;

        default:
            m_nextSceneType = Scene::NONE;
            m_isSceneNeedToBeChanged = false;
        break;
    }
}


void Game::updateUniformBuffer(const mat4s& modelViewProjection) const noexcept
{
    m_windowData.viewport.updateUniformBuffer(modelViewProjection);
}


uint32_t Game::getShaderProgram(const std::string& name) const noexcept
{
    if(auto it = m_shaderPrograms.find(name); it != m_shaderPrograms.end())
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
                auto it = m_shaderPrograms.emplace(name, std::move(program));

                if(it.second)
                    return it.first->second.getHandle();
            }
        }
    }

    return 0;
}


const GameInfo* Game::getInfo() const noexcept
{
    return &m_gameInfo;
}


void Game::updateData() noexcept
{
    if (m_currentScene)
    {
        m_windowData.scene = m_currentScene.get();
        m_currentScene->resize(m_windowData.size.x, m_windowData.size.y);
    }
}