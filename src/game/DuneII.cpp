#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "game/scenes/intro/TitleScreen.hpp"
#include "game/scenes/choosing_houses/Destiny.hpp"
#include "game/DuneII.hpp"


DuneII::DuneII() noexcept:
    m_window(nullptr),
    m_currentScene(nullptr),
    m_nextSceneType(Scene::NONE),
    m_isSceneNeedToBeChanged(false)
{

}


bool DuneII::init(GLFWwindow* window) noexcept
{
    if (!m_currentScene)
    {
        m_window = window;

        glClearColor(0.f, 0.f, 0.f, 1.f);

        auto vbo = glResources.create<GLBuffer, 1>();
        m_uniformBuffer = GLBuffer(vbo[0], GL_UNIFORM_BUFFER);
        m_uniformBuffer.create(sizeof(mat4), 1, nullptr, GL_DYNAMIC_DRAW);
        m_uniformBuffer.bindBufferRange(0, 0, sizeof(mat4));

        m_currentScene = load<TitleScreen>({});
    }
	    
    return m_currentScene ? true : false;
}


void DuneII::update(float dt) noexcept
{
    if(!m_currentScene)
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

            case Scene::Type::CHOOSE_DESTINY:
            {
                if (auto destinyScene = load<Destiny>({}))
                    m_currentScene = destinyScene;
            }
            break;

            case Scene::Type::MISSION:
            {
                
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

    if(m_currentScene)
        m_currentScene->draw();
}


void DuneII::updateUniformBuffer(mat4 modelViewProjection) noexcept
{
    m_uniformBuffer.update(0, sizeof(mat4), 1, static_cast<const void*>(modelViewProjection));
}


void DuneII::click(int button) noexcept
{
    if(m_currentScene)
        m_currentScene->click(button);
}


void DuneII::setCursorPosition(float x, float y) noexcept
{
    if(m_currentScene)
        m_currentScene->setCursorPosition(x, y);
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

        case Scene::Type::CHOOSE_DESTINY:
            if(dynamic_cast<const TitleScreen*>(requester) != nullptr)
            {
                m_isSceneNeedToBeChanged = true;
                m_nextSceneType = Scene::Type::CHOOSE_DESTINY;
            }
        break;

        case Scene::Type::MISSION:
            m_isSceneNeedToBeChanged = true;
        break;

        default:
            m_isSceneNeedToBeChanged = false;
        break;
    }
}