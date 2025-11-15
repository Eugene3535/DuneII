#include "game/scenes/intro/TitleScreen.hpp"
#include "game/DuneII.hpp"


DuneII::DuneII() noexcept:
    m_windowSize(0, 0),
    m_cursorPosition(0, 0),
    m_currentScene(nullptr),
    m_nextSceneType(Scene::NONE),
    m_isSceneNeedToBeChanged(false)
{

}


void DuneII::click(int button) noexcept
{
    if(m_currentScene)
        m_currentScene->click(button);
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


const glm::ivec2& DuneII::getWindowSize() const noexcept
{
	return m_windowSize;
}


const glm::vec2& DuneII::getCursorPosition() const noexcept
{
    return m_cursorPosition;
}


void DuneII::update(float dt) noexcept
{
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
        m_currentScene->resize(m_windowSize);
    }
}


void DuneII::draw() noexcept
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(m_currentScene)
        m_currentScene->draw();
}