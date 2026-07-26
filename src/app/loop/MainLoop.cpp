#include "app/window/MainWindow.hpp"
#include "app/game/Game.hpp"
#include "app/loop/MainLoop.hpp"



MainLoop::MainLoop(MainWindow& window) noexcept:
    m_window(window)
{

}


MainLoop::~MainLoop() = default;


void MainLoop::operator()(Game& game) noexcept
{
    float deltaTime = 0.f;
	float lastFrame = 0.f;

    while (m_window.isOpen())
	{
		m_window.pollEvents();

		const float currentFrame = m_window.getElapsedTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.update(deltaTime);
		game.draw();

		m_window.display();
	}
}