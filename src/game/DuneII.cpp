#include "game/DuneII.hpp"



DuneII::DuneII() noexcept:
    m_windowSize(0, 0),
    m_currentScene(nullptr),
    m_nextSceneType(Scene::NONE),
    m_isSceneNeedToBeChanged(false)
{

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


const glm::ivec2& DuneII::getWindowSize() noexcept
{
	return m_windowSize;
}

