#include "graphics/transform/Transform2D.hpp"


Transform2D::Transform2D() noexcept:
    m_origin(glms_vec2_zero()),
    m_position(glms_vec2_zero()),
    m_scale(glms_vec2_one()),
    m_rotation(0.f)
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
}


void Transform2D::setScale(float x, float y) noexcept
{
    m_scale = { x, y };
}


void Transform2D::setOrigin(float x, float y) noexcept
{
    m_origin = { x, y };
}


void Transform2D::setRotation(float angle) noexcept
{
    m_rotation = static_cast<float>(fmod(angle, 360));

    if (m_rotation < 0)
        m_rotation += 360.f;
}


void Transform2D::setPosition(const vec2s position) noexcept
{
    m_position = position;
}


void Transform2D::setScale(const vec2s factors) noexcept
{
    m_scale = factors;
}


void Transform2D::setOrigin(const vec2s origin) noexcept
{
    m_origin = origin;
}


vec2s Transform2D::getPosition() const noexcept
{
    return m_position;
}


vec2s Transform2D::getScale() const noexcept
{
    return m_scale;
}


vec2s Transform2D::getOrigin() const noexcept
{
    return m_origin;
}


float Transform2D::getRotation() const noexcept
{
    return m_rotation;
}


void Transform2D::move(const vec2s offset) noexcept
{
    const vec2s position = { m_position.x + offset.x, m_position.y + offset.y };
    setPosition(position);
}


void Transform2D::rotate(float angle) noexcept
{
    setRotation(m_rotation + angle);
}


void Transform2D::calculate(mat4 result) const noexcept
{  
    float angle  = glm_rad(-m_rotation);
    float cosine = cos(angle);
    float sine   = sin(angle);
    float sxc    = m_scale.x * cosine;
    float syc    = m_scale.y * cosine;
    float sxs    = m_scale.x * sine;
    float sys    = m_scale.y * sine;
    float tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
    float ty     =  m_origin.x * sxs - m_origin.y * syc + m_position.y;

    auto m = static_cast<float*>(&result[0][0]);

    m[0] = sxc;  m[4] = sys; m[8] = 0.f;  m[12] = tx;
    m[1] = -sxs; m[5] = syc; m[9] = 0.f;  m[13] = ty;
    m[2] = 0.f;  m[6] = 0.f; m[10] = 1.f; m[14] = 0.f;
    m[3] = 0.f;  m[7] = 0.f; m[11] = 0.f; m[15] = 1.f;
}