#include <glad/glad.h>

#include "app/window/MainWindow.hpp"
#include "app/game/Game.hpp"
#include "scenes/intro/TitleScreen.hpp"
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

	game.scenes = std::make_unique<SceneQueue>(&game.scene);

	std::unique_ptr<Scene> titleScreen = std::make_unique<TitleScreen>(&game);

    if (!titleScreen->load({}))
        return;

    game.scenes->push(titleScreen);
	game.updateData();

    while (m_window.isOpen())
	{
		m_window.pollEvents();

		const float currentFrame = m_window.getElapsedTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.scene->update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT);
		auto projection = game.windowData.view->getProjectionMatrix();
		game.scene->draw(projection);

		game.frameCounter++;

		m_window.display();
	}
}