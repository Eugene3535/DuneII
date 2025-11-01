#ifdef DEBUG
#include <cstdio>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game/DuneII.hpp"
#include "game/Application.hpp"


Application::Application() noexcept:
	m_window(nullptr),
	m_width(0),
	m_height(0)
{

}


Application::~Application()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


bool Application::init(const char* title, int width, int height) noexcept
{
	m_width = width;
	m_height = height;

	if (!initWindow(title))
		return false;

	initCallbacks();

	return true;
}


int Application::run(DuneII& game) noexcept
{
	if (!m_window)
		return -1;

	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		// update game

		glfwSwapBuffers(m_window);
	}

	return 0;
}


bool Application::initWindow(const char* title) noexcept
{
	if (glfwInit() == GLFW_TRUE)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr))
		{
			glfwMakeContextCurrent(m_window);
			glfwSwapInterval(1);

			glfwSetWindowUserPointer(m_window, static_cast<void*>(this));

			if (gladLoadGL())
				return true;
		}
	}

	return false;
}


void Application::initCallbacks() noexcept
{
#ifdef DEBUG
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	glDebugMessageCallback([](
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
		{
			fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
		}, nullptr);
#endif

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);

		if (auto app = static_cast<Application*>(glfwGetWindowUserPointer(window)))
		{
			app->m_width = width;
			app->m_height = height;
		}
	});
}