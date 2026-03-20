#include "game/scenes/intro/TitleScreen.hpp"
#include "game/scenes/pick_house/PickHouse.hpp"
// #include "game/scenes/mission/Mission.hpp"
#include "game/DuneII.hpp"


DuneII::DuneII(sf::RenderWindow& rw) noexcept:
    window(rw),
    m_nextSceneType(Scene::NONE),
    m_isSceneNeedToBeChanged(false)
{

}


bool DuneII::init(/* GameState& state */) noexcept
{
    return load<TitleScreen>({});
}


// const GameInfo* DuneII::getInfo() const noexcept
// {
//     return &m_gameInfo;
// }


void DuneII::update(sf::Time dt) noexcept
{
    if (m_currentScene)
        m_currentScene->update(dt);
}


void DuneII::resize(sf::Vector2u size) noexcept
{
    if (m_currentScene)
        m_currentScene->resize(size);
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

    if (m_isSceneNeedToBeChanged)
    {
        switch (m_nextSceneType)
        {
            case Scene::Type::MAIN_MENU:
            {
                load<TitleScreen>({});
            }
            break;

            case Scene::Type::PICK_HOUSE:
            {
                load<PickHouse>({});
            }
            break;

            case Scene::Type::MISSION:
            {
                // if (auto missionScene = load<Mission>("Atreides-8.tmx"))
                //     m_currentScene = missionScene;

                // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            break;

            default:
                break;
        }

        m_currentScene->resize(window.getSize());

        m_isSceneNeedToBeChanged = false;
        m_nextSceneType = Scene::NONE;
    }
}


void DuneII::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_currentScene)
        target.draw(*m_currentScene, states);
}
