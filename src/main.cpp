#ifdef _WIN32
#include <windows.h>
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

#include "application/window/WindowData.hpp"
#include "application/window/MainWindow.hpp"

#include "application/game/Game.hpp"


int main()
{
    WindowData windowData;
    windowData.title = "Dune II: The Battle For Arrakis";
    windowData.size = { 1200, 900 };

    MainWindow window;

    if (!window.open(windowData))
        return 1;

    Game game(windowData);

    if (!game.initialize())
        return 1;
    
    float deltaTime = 0.f;
	float lastFrame = 0.f;
    
	while (window.isOpen())
	{
		window.pollEvents();

		const float currentFrame = window.getElapsedTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.update(deltaTime);
		game.draw();

		window.display();
	}

    return 0;
}