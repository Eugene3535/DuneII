#ifdef DEBUG
#include <cstdio>
#endif

#include <type_traits>

#include <glad/glad.h>

#include "application/context/Context.hpp"



Context::Context() noexcept
{

}


Context::~Context() = default;


bool Context::initialize() noexcept
{
    if (!gladLoadGL())
        return false;

    static_assert(std::is_same_v<GLbyte, int8_t>, "GLbyte mismatch");
    static_assert(std::is_same_v<GLubyte, uint8_t>, "GLubyte mismatch");
    static_assert(std::is_same_v<GLshort, int16_t>, "GLshort mismatch");
    static_assert(std::is_same_v<GLushort, uint16_t>, "GLushort mismatch");
    static_assert(std::is_same_v<GLint, int32_t>, "GLint mismatch");
    static_assert(std::is_same_v<GLuint, uint32_t>, "GLuint mismatch");
    static_assert(std::is_same_v<GLfloat, float>, "GLfloat mismatch");
    static_assert(std::is_same_v<GLdouble, double>, "GLdouble mismatch");

    initializeDebug();

    return true;
}


void Context::initializeDebug() noexcept
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