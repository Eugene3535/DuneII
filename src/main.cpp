#ifdef _WIN32
#include <windows.h>
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

#include "application/game/Game.hpp"
#include "application/window/MainWindow.hpp"


int main()
{
    const char title[] = "Dune II: The Battle For Arrakis";
    int width = 1200;
    int height = 900;

    MainWindow window;
    Game game;
    int retCode = 1;
    
    if (window.open(title, width, height))
        retCode = window.run(game);

    return retCode;
}