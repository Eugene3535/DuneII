#pragma once


#include <cglm/struct/mat4.h>

class OrthoMatrix final
{
public:
    OrthoMatrix() noexcept;
    ~OrthoMatrix();

    void create(int32_t width, int32_t height) noexcept;
    void resize(int32_t width, int32_t height) noexcept;

    void updateUniformBuffer(const mat4s& modelViewProjection) const noexcept;
    void flipVertically(bool flip) noexcept; // If false, the coordinates are counted in OpenGL space - (0, 0) in the lower left corner

    const mat4s&  getProjectionMatrix() const noexcept;
    const ivec2s& getSize()             const noexcept;
    bool          isUpsideDown()        const noexcept;

private:
    mat4s m_projection;
    uint32_t m_uniformBuffer;

    ivec2s m_size;

    bool m_flipVertically;
};