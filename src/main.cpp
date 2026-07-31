#ifdef _WIN32
#include <windows.h>
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

#include "files/FileProvider.hpp"
#include "app/window/WindowData.hpp"
#include "app/window/MainWindow.hpp"
#include "app/context/Context.hpp"
#include "app/loop/MainLoop.hpp"
#include "app/game/Game.hpp"


int main(int argc, char* argv[])
{
    FileProvider provider(argv[0]);

    Game game;
    MainWindow window(game.windowData);

    if (!window.open("Dune II: The Battle For Arrakis", 1200, 900))
        return 1;

    Context context;

	if (!context.load(game))
		return false;

    if (!game.initialize())
        return 1;

    MainLoop loop(window);
    loop(game);

    context.save(game);

    return 0;
}