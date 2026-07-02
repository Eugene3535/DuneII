#ifdef DEBUG
#include <cstdio>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application/context/Context.hpp"
#include "application/game/Game.hpp"
#include "application/window/MainWindow.hpp"


MainWindow::MainWindow() noexcept:
	m_window(nullptr)
{

}


MainWindow::~MainWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


bool MainWindow::open(const char* title, int width, int height) noexcept
{
	if (!createGLFWWindow(title, width, height))
		return false;

	Context context;

	if (!context.initialize())
		return false;

	initCallbacks();

	return true;
}


int MainWindow::run(Game& game) noexcept
{
	if (!m_window)
		return -1;

	glfwSetWindowUserPointer(m_window, static_cast<void*>(&game));
	
	if (!game.init(m_window))
		return 1;

	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	game.resize(width, height);

	float deltaTime = 0.f;
	float lastFrame = 0.f;
    
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.update(deltaTime);
		game.draw();

		glfwSwapBuffers(m_window);
	}

	return 0;
}


bool MainWindow::createGLFWWindow(const char* title, int width, int height) noexcept
{
	if (glfwInit() == GLFW_TRUE)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
    	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

		if (m_window = glfwCreateWindow(width, height, title, nullptr, nullptr); m_window)
		{
			glfwMakeContextCurrent(m_window);
			glfwSwapInterval(1);

			return true;
		}
	}

	return false;
}


void MainWindow::initCallbacks() noexcept
{
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);

		if (auto game = static_cast<Game*>(glfwGetWindowUserPointer(window)); game)
			game->resize(width, height);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
	{
		if (auto game = static_cast<Game*>(glfwGetWindowUserPointer(window)); game)
			game->m_cursorPosition = { static_cast<float>(xpos), static_cast<float>(ypos) };
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, GLFW_TRUE);
	});
}