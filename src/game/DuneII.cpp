#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "resources/files/FileProvider.hpp"
#include "resources/files/Shader.hpp"
#include "game/scenes/intro/TitleScreen.hpp"
#include "game/scenes/pick_house/PickHouse.hpp"
#include "game/scenes/mission/Mission.hpp"
#include "game/DuneII.hpp"


DuneII::DuneII() noexcept:
    m_window(nullptr),
    m_currentScene(nullptr),
    m_nextSceneType(Scene::NONE),
    m_isSceneNeedToBeChanged(false)
{
    m_windowSize = { 0, 0 };
    m_cursorPosition = { 0.f, 0.f };
}


void DuneII::switchScene(const Scene* requester, Scene::Type nextScene) noexcept
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


bool DuneII::isKeyPressed(int key) const noexcept
{
    if(m_window)
        return (glfwGetKey(m_window, key) == GLFW_PRESS);

    return false;
}


bool DuneII::isMouseButtonPressed(int button) const noexcept
{
    if (m_window)
        return (glfwGetMouseButton(m_window, button) == GLFW_PRESS);

    return false;
}


uint32_t DuneII::getShaderProgram(const std::string& name) const noexcept
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


const vec2s& DuneII::getCursorPosition() const noexcept
{
    return m_cursorPosition;
}


const ivec2s& DuneII::getWindowsSize() const noexcept
{
    return m_windowSize;
}


bool DuneII::init(GLFWwindow* window) noexcept
{
    if (!m_currentScene)
    {
        m_window = window;
        camera.init();

        glClearColor(0.f, 0.f, 0.f, 1.f);

        m_currentScene = load<TitleScreen>({});
    }

    return m_currentScene ? true : false;
}


void DuneII::update(float dt) noexcept
{
    if (!m_currentScene)
        return;

    m_currentScene->update(dt);

    if (m_isSceneNeedToBeChanged)
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

                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            break;

            default:
                break;
        }

        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        m_currentScene->resize(width, height);

        m_isSceneNeedToBeChanged = false;
        m_nextSceneType = Scene::NONE;
    }
}


void DuneII::draw() noexcept
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_currentScene)
        m_currentScene->draw();
}


void DuneII::resize(int width, int height) noexcept
{
    m_windowSize = { width, height };
    camera.setupProjectionMatrix(width, height);

    if (m_currentScene)
        m_currentScene->resize(width, height);
}