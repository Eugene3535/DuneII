#ifdef _WIN32
#include <windows.h>
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

#include "game/Engine.hpp"
#include "application/MainWindow.hpp"


int main()
{
    const char title[] = "Dune II: The Battle For Arrakis";
    int width = 1200;
    int height = 900;

    MainWindow app;
    Engine engine;
    int retCode = -1;
    
    if(app.create(title, width, height))
        retCode = app.run(engine);

    return retCode;
}