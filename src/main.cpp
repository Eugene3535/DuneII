#ifdef _WIN32
#include <windows.h>
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

#include "game/DuneII.hpp"
#include "app/Application.hpp"


int main()
{
    const char title[] = "Dune II: The Battle For Arrakis";
    int width = 800;
    int height = 600;

    Application app;
    DuneII game;
    int retCode = -1;
    
    if(app.init(title, width, height))
        retCode = app.run(game);

    return retCode;
}