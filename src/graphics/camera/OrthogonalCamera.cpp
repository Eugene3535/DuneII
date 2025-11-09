
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "graphics/Transform2D.hpp"
#include "graphics/camera/OrthogonalCamera.hpp"


OrthogonalCamera::OrthogonalCamera() noexcept:
    m_projection(glm::identity<glm::mat4>()),
    m_modelViewNeedUpdate(true),
    m_flipVertically(true)
{
    
}


void OrthogonalCamera::setupProjectionMatrix(int32_t width, int32_t height) noexcept
{
    m_projection = m_flipVertically ? glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f):
                                      glm::ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height));
}


void OrthogonalCamera::flipVertically(bool flip) noexcept
{
    m_flipVertically = flip;
}


glm::mat4 OrthogonalCamera::getModelViewProjectionMatrix() const noexcept
{
    return m_projection * getMatrix();
}


bool OrthogonalCamera::isUpsideDown() const noexcept
{
    return m_flipVertically;
}