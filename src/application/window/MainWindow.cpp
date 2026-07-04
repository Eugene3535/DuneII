#ifdef DEBUG
#include <cstdio>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application/context/Context.hpp"
#include "application/window/WindowData.hpp"
#include "application/window/MainWindow.hpp"
#include "scenes/Scene.hpp"


MainWindow::MainWindow() noexcept:
	m_glfwWindow(nullptr)
{

}


MainWindow::~MainWindow()
{
	glfwDestroyWindow(m_glfwWindow);
	glfwTerminate();
}


bool MainWindow::open(WindowData& data) noexcept
{
	if (!createGLFWWindow(data.title.c_str(), data.size.x, data.size.y))
		return false;

	Context context;

	if (!context.initialize())
		return false;

	data.viewport.create(data.size.x, data.size.y);
	initCallbacks(data);

	return true;
}


void MainWindow::close() const noexcept
{
    glfwSetWindowShouldClose(m_glfwWindow, GLFW_TRUE);
}


void MainWindow::pollEvents() const noexcept
{
    glfwPollEvents();
}


void MainWindow::display() const noexcept
{
    glfwSwapBuffers(m_glfwWindow);
}


float MainWindow::getElapsedTime() const noexcept
{
    return static_cast<float>(glfwGetTime());
}


ivec2s MainWindow::getSize() const noexcept
{
    int width, height;
	glfwGetWindowSize(m_glfwWindow, &width, &height);

    return { width, height };
}


bool MainWindow::isOpen() const noexcept
{
	return static_cast<bool>(!glfwWindowShouldClose(m_glfwWindow));
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

		if (m_glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr); m_glfwWindow)
		{
			glfwMakeContextCurrent(m_glfwWindow);
			glfwSwapInterval(1);

			return true;
		}
	}

	return false;
}


void MainWindow::initCallbacks(WindowData& data) noexcept
{
	glfwSetWindowUserPointer(m_glfwWindow, static_cast<void*>(&data));

	glfwSetFramebufferSizeCallback(m_glfwWindow, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);

		if (auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)); data)
		{
			data->size = { width, height };
			data->viewport.resize(width, height);

			if (data->scene)
				static_cast<Scene*>(data->scene)->resize(width, height);
		}
	});

	glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
	{
		if (auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)); data)
		{
			if (data->scene)
				static_cast<Scene*>(data->scene)->setCursor(static_cast<float>(xpos), static_cast<float>(ypos));
		}
	});

	glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
	{
		if (auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)); data)
		{
			if (data->scene)
				static_cast<Scene*>(data->scene)->setMouse(button, action, mods);
		}
	});

	glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, GLFW_TRUE);

		if (auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)); data)
		{
			if (data->scene)
				static_cast<Scene*>(data->scene)->setKeyboard(key, scancode, action, mods);
		}
	});
}