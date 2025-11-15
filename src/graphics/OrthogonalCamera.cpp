#include <cglm/call/cam.h>

#include "graphics/OrthogonalCamera.hpp"


OrthogonalCamera::OrthogonalCamera() noexcept:
    Transform2D(),
    m_modelViewNeedUpdate(true),
    m_flipVertically(true)
{
    glmc_mat4_identity(m_projection);
}


void OrthogonalCamera::setupProjectionMatrix(int32_t width, int32_t height) noexcept
{
    if(m_flipVertically)
        glmc_ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f, m_projection);
    else
        glmc_ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height), -1.f, 1.f, m_projection);
}


void OrthogonalCamera::flipVertically(bool flip) noexcept
{
    m_flipVertically = flip;
}


void OrthogonalCamera::getModelViewProjectionMatrix(mat4 mvp) noexcept
{
    mat4 model_view;
    getMatrix(model_view);
    glmc_mat4_mul(m_projection, model_view, mvp);
}


bool OrthogonalCamera::isUpsideDown() const noexcept
{
    return m_flipVertically;
}