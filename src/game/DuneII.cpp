#include "scenes/intro/MainMenu.hpp"
#include "scenes/mission/Mission.hpp"
#include "game/DuneII.hpp"


struct 
{
    bool isSceneNeedToBeChanged = false;
    DuneII::GameScene nextScene = DuneII::NONE;
} game_state;


AssetManager& DuneII::getAssets() noexcept
{
    return m_assets;
}


void DuneII::notifyChangeScene(const Scene* requester, GameScene requested_scene) noexcept
{
    if(game_state.nextScene == GameScene::NONE)
    {
        switch (requested_scene)
        {
            case GameScene::MAIN_MENU:

                if(dynamic_cast<const Mission*>(requester))
                {
                    game_state.isSceneNeedToBeChanged = true;
                    game_state.nextScene = requested_scene;
                }
            
            break;

            case GameScene::MISSION:

                if(dynamic_cast<const MainMenu*>(requester))
                {
                    game_state.isSceneNeedToBeChanged = true;
                    game_state.nextScene = requested_scene;
                }

            break;
            
            default:
                break;
        }
    }
}


std::pair<DuneII::GameScene, bool> DuneII::isSceneNeedToBeChanged() const noexcept
{
    if(game_state.isSceneNeedToBeChanged)
        return { game_state.nextScene, true };

    return { DuneII::GameScene::NONE, false };
}


void DuneII::resetSceneChange() noexcept
{
    if(game_state.isSceneNeedToBeChanged)
    {
        game_state.isSceneNeedToBeChanged = false;
        game_state.nextScene = GameScene::NONE;
    }
}