#ifdef DEBUG
#include <cstdio>
#endif

#include <type_traits>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game/scenes/mission/Mission.hpp"
#include "game/Engine.hpp"
#include "app/Game.hpp"


Game::Game() noexcept:
	m_window(nullptr)
{

}


Game::~Game()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


bool Game::init(const char* title, int width, int height) noexcept
{
	if (!initWindow(title, width, height))
		return false;

	checkOpenglTypes();
	initCallbacks();

	return true;
}


int Game::run(Engine& engine) noexcept
{
	if (!m_window)
		return -1;

	glfwSetWindowUserPointer(m_window, static_cast<void*>(&engine));
	
	if(!engine.init(m_window))
		return 1;

	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	engine.resize(width, height);

	float deltaTime = 0.f;
	float lastFrame = 0.f;
    
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		engine.update(deltaTime);
		engine.draw();

		glfwSwapBuffers(m_window);
	}

	return 0;
}


bool Game::initWindow(const char* title, int width, int height) noexcept
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


void Game::checkOpenglTypes() noexcept
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


void Game::initCallbacks() noexcept
{
#ifdef DEBUG
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		auto source_str = [source]() -> const char*
		{
			switch (source)
			{
				case GL_DEBUG_SOURCE_API:             return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW SYSTEM";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
				case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD PARTY";
				case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
				case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
				
				default: return "UNKNOWN";
			}
		}();

		auto type_str = [type]() -> const char*
		{
			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR:               return "ERROR";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
				case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
				case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
				case GL_DEBUG_TYPE_MARKER:              return "MARKER";
				case GL_DEBUG_TYPE_OTHER:               return "OTHER";

				default: return "UNKNOWN";
			}
		}();

		auto severity_str = [severity]() -> const char*
		{
			switch (severity) 
			{
				case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
				case GL_DEBUG_SEVERITY_LOW:          return "LOW";
				case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
				case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";

				default: return "UNKNOWN";
			}
		}();

		printf("%s, %s, %s, %u: %s\n", source_str, type_str, severity_str, id, message);
	}, nullptr);
#endif

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);

		if(auto engine = static_cast<Engine*>(glfwGetWindowUserPointer(window)))
			engine->resize(width, height);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
	{
		if(auto engine = static_cast<Engine*>(glfwGetWindowUserPointer(window)))
			engine->m_cursorPosition = { static_cast<float>(xpos), static_cast<float>(ypos) };
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, GLFW_TRUE);
	});
}