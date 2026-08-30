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
	float lastFrame = 0.f;

	Scene* scene { nullptr };
	SceneManager sceneManager(&scene, &game.windowData);
	game.sceneManager = &sceneManager;
	{
		std::unique_ptr<Scene> titleScreen = std::make_unique<TitleScreen>(&game);

		if (!titleScreen->load({}))
			return;

		sceneManager.push(titleScreen);
	}

	auto& tasks = game.tasks;

    while (m_window.isOpen())
	{
		m_window.pollEvents();

		const float currentFrame = m_window.getElapsedTime();
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		for (size_t i = 0; i < tasks.size(); ++i)
		{
			auto [data, func] = tasks[i];
			auto result = func(data, deltaTime);

			if (result)
			{
				game.taskManager.release(data, result);
				std::swap(tasks[i], tasks.back());
				tasks.pop_back();
			}
		}

		scene->update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT);
		auto projection = game.windowData.view->getProjectionMatrix();
		scene->draw(projection);

		game.frameCounter++;

		m_window.display();
	}
}