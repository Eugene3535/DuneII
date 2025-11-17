#include "game/scenes/intro/TitleScreen.hpp"
#include "game/DuneII.hpp"


DuneII::DuneII() noexcept:
    m_currentScene(nullptr),
    m_nextSceneType(Scene::NONE),
    m_isSceneNeedToBeChanged(false)
{

}


bool DuneII::init() noexcept
{
    if (!m_currentScene)
    {
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
                
            }
            break;

            case Scene::Type::MISSION:
            {
                
            }
            break;

            default:
                break;
        }

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
            m_isSceneNeedToBeChanged = true;
        break;

        case Scene::Type::MISSION:
            m_isSceneNeedToBeChanged = true;
        break;

        default:
            m_isSceneNeedToBeChanged = false;
        break;
    }
}