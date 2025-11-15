#include <cglm/call/vec2.h>

#include "graphics/Transform2D.hpp"


Transform2D::Transform2D() noexcept:
    m_rotation(0),
    m_transformNeedUpdate(true)
{
    glmc_vec2_zero(m_origin);
    glmc_vec2_zero(m_position);
    glmc_vec2_one(m_scale);
    glmc_mat4_identity(m_matrix);
}


Transform2D::~Transform2D() = default;



void Transform2D::loadIdentity() noexcept
{
    *this = Transform2D();
}


void Transform2D::setPosition(float x, float y) noexcept
{
    m_position[0] = x;
    m_position[1] = y;
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
    m_origin[0] = x;
    m_origin[1] = y;
    m_transformNeedUpdate = true;
}


void Transform2D::setPosition(const vec2 position) noexcept
{
    m_position[0] = position[0];
    m_position[1] = position[1];
    m_transformNeedUpdate = true;
}


void Transform2D::setRotation(float angle) noexcept
{
    m_rotation = static_cast<float>(fmod(angle, 360));

    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformNeedUpdate = true;
}


void Transform2D::setScale(const vec2 factors) noexcept
{
    m_scale[0] = factors[0];
    m_scale[1] = factors[1];
    m_transformNeedUpdate = true;
}


void Transform2D::setOrigin(const vec2 origin) noexcept
{
    m_origin[0] = origin[0];
    m_origin[1] = origin[1];
    m_transformNeedUpdate = true;
}


void Transform2D::getPosition(vec2 position) const noexcept
{
    position[0] = m_position[0];
    position[1] = m_position[1];
}


float Transform2D::getRotation() const noexcept
{
    return m_rotation;
}


void Transform2D::getScale(vec2 scale) const noexcept
{
    scale[0] = m_scale[0];
    scale[1] = m_scale[1];
}


void Transform2D::getOrigin(vec2 origin) const noexcept
{
    origin[0] = m_origin[0];
    origin[1] = m_origin[1];
}


void Transform2D::move(const vec2 offset) noexcept
{
    vec2 position = { m_position[0] + offset[0], m_position[1] + offset[1] };
    setPosition(position);
}


void Transform2D::rotate(float angle) noexcept
{
    setRotation(m_rotation + angle);
}


void Transform2D::getMatrix(mat4 result) const noexcept
{  
    // Recompute the matrix if needed
    if (m_transformNeedUpdate)
    {
        float angle  = glm_rad(-m_rotation);
        float cosine = cos(angle);
        float sine   = sin(angle);
        float sxc    = m_scale[0] * cosine;
        float syc    = m_scale[1] * cosine;
        float sxs    = m_scale[0] * sine;
        float sys    = m_scale[1] * sine;
        float tx     = -m_origin[0] * sxc - m_origin[1] * sys + m_position[0];
        float ty     =  m_origin[0] * sxs - m_origin[1] * syc + m_position[1];

        auto m = static_cast<float*>(&m_matrix[0][0]);

        m[0] = sxc;  m[4] = sys; m[8] = 0.f;  m[12] = tx;
        m[1] = -sxs; m[5] = syc; m[9] = 0.f;  m[13] = ty;
        m[2] = 0.f;  m[6] = 0.f; m[10] = 1.f; m[14] = 0.f;
        m[3] = 0.f;  m[7] = 0.f; m[11] = 0.f; m[15] = 1.f;

        m_transformNeedUpdate = false;
    }

    glmc_mat4_copy(m_matrix, result);
}