#include <glad/glad.h>
#include <cglm/struct/cam.h>
#include "cglm/struct/affine-mat.h"

#include "graphics/camera/OrthogonalCamera.hpp"


OrthogonalCamera::OrthogonalCamera() noexcept:
    Transform2D(),
    m_projection(glms_mat4_identity()),
    m_uniformBuffer(0),
    m_flipVertically(true)
{

}


OrthogonalCamera::~OrthogonalCamera()
{
    if(m_uniformBuffer)
        glDeleteBuffers(1, &m_uniformBuffer);
}


void OrthogonalCamera::init() noexcept
{
    if (!m_uniformBuffer)
    {
        glCreateBuffers(1, &m_uniformBuffer);
        glNamedBufferData(m_uniformBuffer, sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uniformBuffer, 0, sizeof(mat4));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}


void OrthogonalCamera::setupProjectionMatrix(int32_t width, int32_t height) noexcept
{
    m_projection = m_flipVertically ? 
        glms_ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f) :
        glms_ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height), -1.f, 1.f);
}


void OrthogonalCamera::updateUniformBuffer(mat4 modelViewProjection) const noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), static_cast<const void*>(modelViewProjection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void OrthogonalCamera::flipVertically(bool flip) noexcept
{
    m_flipVertically = flip;
}


mat4s OrthogonalCamera::getModelViewProjectionMatrix() noexcept
{
    mat4s modelView = getMatrix();

    return glms_mul(m_projection, modelView);
}


bool OrthogonalCamera::isUpsideDown() const noexcept
{
    return m_flipVertically;
}