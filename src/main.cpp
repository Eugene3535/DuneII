#ifdef _WIN32
#include <windows.h>
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

#include "game/Engine.hpp"
#include "app/Game.hpp"


int main()
{
    const char title[] = "Dune II: The Battle For Arrakis";
    int width = 1200;
    int height = 900;

    Game game;
    Engine engine;
    int retCode = -1;
    
    if(game.init(title, width, height))
        retCode = game.run(engine);

    return retCode;
}