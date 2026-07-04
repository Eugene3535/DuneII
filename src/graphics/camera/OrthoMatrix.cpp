#include <glad/glad.h>
#include <cglm/struct/cam.h>
#include "cglm/struct/affine-mat.h"

#include "graphics/camera/OrthoMatrix.hpp"


OrthoMatrix::OrthoMatrix() noexcept:
    m_projection(glms_mat4_identity()),
    m_uniformBuffer(0),
    m_flipVertically(true)
{

}


OrthoMatrix::~OrthoMatrix()
{
    if(m_uniformBuffer)
        glDeleteBuffers(1, &m_uniformBuffer);
}


void OrthoMatrix::create(int32_t width, int32_t height) noexcept
{
    if (!m_uniformBuffer)
    {
        glCreateBuffers(1, &m_uniformBuffer);
        glNamedBufferData(m_uniformBuffer, sizeof(mat4s), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uniformBuffer, 0, sizeof(mat4s));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    resize(width, height);
}


void OrthoMatrix::resize(int32_t width, int32_t height) noexcept
{
    m_projection = m_flipVertically ? 
        glms_ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f) :
        glms_ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height), -1.f, 1.f);
}


void OrthoMatrix::updateUniformBuffer(const mat4s& modelViewProjection) const noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4s), static_cast<const void*>(modelViewProjection.raw));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void OrthoMatrix::flipVertically(bool flip) noexcept
{
    m_flipVertically = flip;
}


const mat4s& OrthoMatrix::getProjectionMatrix() const noexcept
{
    return m_projection;
}


bool OrthoMatrix::isUpsideDown() const noexcept
{
    return m_flipVertically;
}