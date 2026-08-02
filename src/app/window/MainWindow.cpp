#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/camera/OrthoMatrix.hpp"
#include "app/window/WindowData.hpp"
#include "app/window/MainWindow.hpp"
#include "scenes/Scene.hpp"


static GLFWwindow* s_glfwWindow;


MainWindow::MainWindow(WindowData& data) noexcept:
	m_windowData(data),
	m_glfwWindow(nullptr)
{
	
}


MainWindow::~MainWindow()
{
	glfwDestroyWindow(m_glfwWindow);
	glfwTerminate();
}


bool MainWindow::open(const char* title, int width, int height) noexcept
{
	if (!createGLFWWindow(title, width, height))
		return false;

	s_glfwWindow = m_glfwWindow;

    if (!gladLoadGL())
        return false;

	initializeDebug();
	glClearColor(0.f, 0.f, 0.f, 1.f);

	m_camera = std::make_unique<OrthoMatrix>();
	m_camera->create(width, height);
	m_windowData.view = m_camera.get();

	initCallbacks();

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


bool MainWindow::isKeyPressed(int key) noexcept
{
	return s_glfwWindow ? (glfwGetKey(s_glfwWindow, key) != GLFW_RELEASE) : false;
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
			// glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			return true;
		}
	}

	return false;
}


void MainWindow::initializeDebug() noexcept
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
}


void MainWindow::initCallbacks() noexcept
{
	glfwSetWindowUserPointer(m_glfwWindow, static_cast<void*>(&m_windowData));

	glfwSetFramebufferSizeCallback(m_glfwWindow, [](GLFWwindow* window, int width, int height)
	{
		if (glViewport)
			glViewport(0, 0, width, height);

		if (auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)))
		{
			data->view->resize(width, height);

			if (data->scene)
				data->scene->resize(width, height);
		}
	});

	glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
	{
		if (auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)))
		{
			data->cursor = { static_cast<float>(xpos), static_cast<float>(ypos) };
		}
	});

	glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
	{
		if (auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)))
		{
			data->mouse.button = button;
			data->mouse.action = action;
			data->mouse.mods = mods;
		}
	});

	glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, GLFW_TRUE);

		if (auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)))
		{
			data->keyboard.key = key;
			data->keyboard.scancode = scancode;
			data->keyboard.action = action;
			data->keyboard.mods = mods;
		}
	});
}