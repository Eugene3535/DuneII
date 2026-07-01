#pragma once

#include <cglm/struct/vec2.h>
#include <cglm/struct/mat4.h>


// Class for optimized use of orthographic matrix
class Transform2D
{
public:
    Transform2D() noexcept;
    virtual ~Transform2D();

    void loadIdentity() noexcept;

    void setPosition(float x, float y) noexcept;
    void setScale(float x, float y)    noexcept;
    void setOrigin(float x, float y)   noexcept;
    void setRotation(float angle)      noexcept;

    void setPosition(const vec2s position) noexcept;
    void setScale(const vec2s factors)     noexcept;
    void setOrigin(const vec2s origin)     noexcept;

    vec2s getPosition() const noexcept;
    vec2s getScale()    const noexcept;
    vec2s getOrigin()   const noexcept;
    float getRotation() const noexcept;

    void move(const vec2s offset) noexcept;
    void rotate(float angle)      noexcept;

    const mat4s& getMatrix() const noexcept;

private:
    mutable mat4s m_matrix;
    vec2s         m_origin;
    vec2s         m_position;       
    vec2s         m_scale;
    float         m_rotation;
    mutable bool  m_transformNeedUpdate;
};