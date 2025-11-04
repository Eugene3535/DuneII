#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/Transform2D.hpp"


Transform2D::Transform2D() noexcept:
    m_matrix(glm::identity<glm::mat4>()),
    m_origin(0, 0),
    m_position(0, 0),         
    m_scale(1, 1),
    m_rotation(0),
    m_transformNeedUpdate(true)
{

}


Transform2D::~Transform2D() = default;



void Transform2D::loadIdentity() noexcept
{
    *this = Transform2D();
}


void Transform2D::setPosition(float x, float y) noexcept
{
    m_position = { x, y };
    m_transformNeedUpdate = true;
}


void Transform2D::setScale(float x, float y) noexcept
{
    m_scale[0] = x;
    m_scale[1] = y;
    m_transformNeedUpdate = true;
}


void Transform2D::setOrigin(float x, float y) noexcept
{
    m_origin = { x, y };
    m_transformNeedUpdate = true;
}


void Transform2D::setPosition(const glm::vec2& position) noexcept
{
    m_position = position;
    m_transformNeedUpdate = true;
}


void Transform2D::setRotation(float angle) noexcept
{
    m_rotation = static_cast<float>(fmod(angle, 360));

    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformNeedUpdate = true;
}


void Transform2D::setScale(const glm::vec2& factors) noexcept
{
    m_scale = factors;
    m_transformNeedUpdate = true;
}


void Transform2D::setOrigin(const glm::vec2& origin) noexcept
{
    m_origin = origin;
    m_transformNeedUpdate = true;
}


const glm::vec2& Transform2D::getPosition() const noexcept
{
    return m_position;
}


float Transform2D::getRotation() const noexcept
{
    return m_rotation;
}


const glm::vec2& Transform2D::getScale() const noexcept
{
    return m_scale;
}


const glm::vec2& Transform2D::getOrigin() const noexcept
{
    return m_origin;
}


void Transform2D::move(const glm::vec2& offset) noexcept
{
    m_position += offset;
    m_transformNeedUpdate = true;
}


void Transform2D::rotate(float angle) noexcept
{
    setRotation(m_rotation + angle);
    m_transformNeedUpdate = true;
}


const glm::mat4& Transform2D::getMatrix() const noexcept
{  
    // Recompute the matrix if needed
    if (m_transformNeedUpdate)
    {
        float angle  = glm::radians(-m_rotation);
        float cosine = cos(angle);
        float sine   = sin(angle);
        float sxc    = m_scale[0] * cosine;
        float syc    = m_scale[1] * cosine;
        float sxs    = m_scale[0] * sine;
        float sys    = m_scale[1] * sine;
        float tx     = -m_origin[0] * sxc - m_origin[1] * sys + m_position[0];
        float ty     =  m_origin[0] * sxs - m_origin[1] * syc + m_position[1];

        auto m = static_cast<float*>(glm::value_ptr(m_matrix));

        m[0] = sxc;  m[4] = sys; m[8] = 0.f;  m[12] = tx;
        m[1] = -sxs; m[5] = syc; m[9] = 0.f;  m[13] = ty;
        m[2] = 0.f;  m[6] = 0.f; m[10] = 1.f; m[14] = 0.f;
        m[3] = 0.f;  m[7] = 0.f; m[11] = 0.f; m[15] = 1.f;

        m_transformNeedUpdate = false;
    }

    return m_matrix;
}