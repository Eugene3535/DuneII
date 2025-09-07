#include "scenes/intro/TitleScreen.hpp"
#include "scenes/mission/Mission.hpp"
#include "game/DuneII.hpp"


struct 
{
    bool isSceneNeedToBeChanged = false;
} game_state;


AssetManager& DuneII::getAssets() noexcept
{
    return m_assets;
}


bool DuneII::checkSceneRights(const Scene* requester, Scene::Type requestedType) noexcept
{
    switch (requestedType)
    {
        case Scene::Type::MAIN_MENU:

            if(dynamic_cast<const Mission*>(requester))
                return true;
            
        case Scene::Type::MISSION:

            if(dynamic_cast<const TitleScreen*>(requester))
                return true;
        
        default:
            return false;
    }
    
}