#include <cassert>

#include "graphics/Mesh.hpp"


void Sprite::draw() noexcept
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLE_FAN, frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void SpriteSheet::draw() noexcept
{
    assert((currentFrame < frames.size()));

    if( ! frames.empty() )
    {
        GLuint frame = frames[currentFrame];
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLE_FAN, frame, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
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