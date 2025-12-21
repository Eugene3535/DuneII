#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

}


void DuneII::resize(int width, int height) noexcept
{
    camera.setupProjectionMatrix(width, height);
    
    if(m_currentScene)
        m_currentScene->resize(width, height);
}


void DuneII::switchScene(const Scene* requester, Scene::Type nextScene) noexcept
{
    switch (nextScene)
    {
        case Scene::Type::MAIN_MENU:
            m_isSceneNeedToBeChanged = true;
        break;

        case Scene::Type::PICK_HOUSE:
            if(dynamic_cast<const TitleScreen*>(requester) != nullptr)
            {
                m_isSceneNeedToBeChanged = true;
                m_nextSceneType = Scene::Type::PICK_HOUSE;
            }
        break;

        case Scene::Type::MISSION:
            if(dynamic_cast<const PickHouse*>(requester) != nullptr)
            {
                m_isSceneNeedToBeChanged = true;
                m_nextSceneType = Scene::Type::MISSION;
            }
        break;

        default:
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


vec2s DuneII::getCursorPosition() const noexcept
{
    double xpos = 0, ypos = 0;

    if(m_window)
        glfwGetCursorPos(m_window, &xpos, &ypos);

    return { static_cast<float>(xpos), static_cast<float>(ypos) };
}


ivec2s DuneII::getWindowsSize() const noexcept
{
    int width = 0;
    int height = 0;

    if(m_window)
        glfwGetWindowSize(m_window, &width, &height);

    return { width, height };
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