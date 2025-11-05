#ifndef TRANSFORM_2D_HPP
#define TRANSFORM_2D_HPP

#include <glm/vec2.hpp> 
#include <glm/mat4x4.hpp>

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

    void setPosition(const glm::vec2& position) noexcept;
    void setRotation(float angle)               noexcept;
    void setScale(const glm::vec2& factors)     noexcept;
    void setOrigin(const glm::vec2& origin)     noexcept;

    const glm::vec2& getPosition() const noexcept;
    float            getRotation() const noexcept;
    const glm::vec2& getScale()    const noexcept;
    const glm::vec2& getOrigin()   const noexcept;

    void move(const glm::vec2& offset) noexcept;
    void rotate(float angle)     noexcept;

    const glm::mat4& getMatrix() const noexcept;

private:
    mutable glm::mat4 m_matrix;
    glm::vec2         m_origin;
    glm::vec2         m_position;         
    glm::vec2         m_scale;
    float             m_rotation;      
    mutable bool      m_transformNeedUpdate;       
};

#endif 