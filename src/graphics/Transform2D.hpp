#ifndef TRANSFORM_2D_HPP
#define TRANSFORM_2D_HPP

#include <cglm/types.h>
#include <cglm/call/mat4.h>


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

    void setPosition(const vec2 position) noexcept;
    void setRotation(float angle)         noexcept;
    void setScale(const vec2 factors)     noexcept;
    void setOrigin(const vec2 origin)     noexcept;

    void  getPosition(vec2 position) const noexcept;
    float getRotation()              const noexcept;
    void  getScale(vec2 scale)       const noexcept;
    void  getOrigin(vec2 origin)     const noexcept;

    void move(const vec2 offset) noexcept;
    void rotate(float angle)     noexcept;

    void getMatrix(mat4 result) const noexcept;

private:
    mutable mat4 m_matrix;
    vec2         m_origin;
    vec2         m_position;         
    vec2         m_scale;
    float        m_rotation;      
    mutable bool m_transformNeedUpdate;       
};

#endif 