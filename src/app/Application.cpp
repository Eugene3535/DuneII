#ifdef DEBUG
#include <cstdio>
#endif

#include <type_traits>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game/DuneII.hpp"
#include "app/Application.hpp"


Application::Application() noexcept:
	m_window(nullptr)
{

}


Application::~Application()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


bool Application::init(const char* title, int width, int height) noexcept
{
	if (!initWindow(title, width, height))
		return false;

	checkOpenglTypes();
	initCallbacks();

	return true;
}


int Application::run(DuneII& game) noexcept
{
	if (!m_window)
		return -1;

	glfwSetWindowUserPointer(m_window, static_cast<void*>(&game));
	
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	game.camera.setupProjectionMatrix(width, height);

	if(!game.init(m_window))
		return 1;

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


bool Application::initWindow(const char* title, int width, int height) noexcept
{
	if (glfwInit() == GLFW_TRUE)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (m_window = glfwCreateWindow(width, height, title, nullptr, nullptr))
		{
			glfwMakeContextCurrent(m_window);
			glfwSwapInterval(1);

			if (gladLoadGL())
				return true;
		}
	}

	return false;
}


void Application::checkOpenglTypes() noexcept
{
    static_assert(std::is_same_v<GLbyte, int8_t>, "GLbyte mismatch");
    static_assert(std::is_same_v<GLubyte, uint8_t>, "GLubyte mismatch");
    static_assert(std::is_same_v<GLshort, int16_t>, "GLshort mismatch");
    static_assert(std::is_same_v<GLushort, uint16_t>, "GLushort mismatch");
    static_assert(std::is_same_v<GLint, int32_t>, "GLint mismatch");
    static_assert(std::is_same_v<GLuint, uint32_t>, "GLuint mismatch");
    static_assert(std::is_same_v<GLfloat, float>, "GLfloat mismatch");
    static_assert(std::is_same_v<GLdouble, double>, "GLdouble mismatch");

#ifdef DEBUG
    printf("All OpenGL types match expected types\n");
#endif
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

		if (auto game = static_cast<DuneII*>(glfwGetWindowUserPointer(window)))
			game->resize(width, height);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
	{
		if (auto game = static_cast<DuneII*>(glfwGetWindowUserPointer(window)))
			game->setCursorPosition(static_cast<float>(xpos), static_cast<float>(ypos));	
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
	{
		if(action == GLFW_PRESS)
		{
			if (auto game = static_cast<DuneII*>(glfwGetWindowUserPointer(window)))
				game->click(button);
		}
	});
}