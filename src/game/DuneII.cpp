#include "scenes/intro/MainMenu.hpp"
#include "game/DuneII.hpp"


struct GameState
{

};


DuneII::DuneII() noexcept
{

}


DuneII::~DuneII() = default;


void DuneII::changeScene(const Scene* requester, GameScene nextScene) noexcept
{
    switch (nextScene)
    {
    case GameScene::MAIN_MENU:
        /* code */
        break;

    case GameScene::MISSION:
        /* code */
        break;
    
    default:
        break;
    }
}