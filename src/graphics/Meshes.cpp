#include <cassert>

#include <glad/glad.h>

#include "graphics/Meshes.hpp"


void Sprite::draw() noexcept
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLE_FAN, frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Animation::draw() noexcept
{
    assert( (currentFrame < end) && "Frame is out of range" );
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLE_FAN, currentFrame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Shape::draw() noexcept
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLE_FAN, 0, count);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Plane::draw() noexcept
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}