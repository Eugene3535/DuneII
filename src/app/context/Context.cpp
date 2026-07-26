#ifdef DEBUG
#include <cstdio>
#endif

#include <type_traits>

#include "app/game/Game.hpp"
#include "app/context/Context.hpp"



Context::Context() noexcept
{
    static_assert(std::is_same_v<GLbyte, int8_t>, "GLbyte mismatch");
    static_assert(std::is_same_v<GLubyte, uint8_t>, "GLubyte mismatch");
    static_assert(std::is_same_v<GLshort, int16_t>, "GLshort mismatch");
    static_assert(std::is_same_v<GLushort, uint16_t>, "GLushort mismatch");
    static_assert(std::is_same_v<GLint, int32_t>, "GLint mismatch");
    static_assert(std::is_same_v<GLuint, uint32_t>, "GLuint mismatch");
    static_assert(std::is_same_v<GLfloat, float>, "GLfloat mismatch");
    static_assert(std::is_same_v<GLdouble, double>, "GLdouble mismatch");
}


Context::~Context() = default;


bool Context::load(Game& game) noexcept
{
    return true;
}


void Context::save(Game& game) noexcept
{

}