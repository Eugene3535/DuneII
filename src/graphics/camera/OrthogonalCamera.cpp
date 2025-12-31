#include <glad/glad.h>
#include <cglm/call/cam.h>
#include "cglm/struct/affine-mat.h"

#include "graphics/camera/OrthogonalCamera.hpp"


OrthogonalCamera::OrthogonalCamera() noexcept:
    Transform2D(),
    m_uniformBuffer(0),
    m_modelViewNeedUpdate(true),
    m_flipVertically(true)
{
    glmc_mat4_identity(m_projection);
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
        glGenBuffers(1, &m_uniformBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uniformBuffer, 0, sizeof(mat4));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}


void OrthogonalCamera::setupProjectionMatrix(int32_t width, int32_t height) noexcept
{
    if(m_flipVertically)
        glmc_ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f, m_projection);
    else
        glmc_ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height), -1.f, 1.f, m_projection);
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


void OrthogonalCamera::getModelViewProjectionMatrix(mat4 mvp) noexcept
{
    mat4s modelView = getMatrix();
    glmc_mat4_mul(m_projection, modelView.raw, mvp);
}


bool OrthogonalCamera::isUpsideDown() const noexcept
{
    return m_flipVertically;
}