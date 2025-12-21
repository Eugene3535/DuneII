#ifndef ORTHOGONAL_CAMERA_HPP
#define ORTHOGONAL_CAMERA_HPP

#include "graphics/transform/Transform2D.hpp"

class OrthogonalCamera final:
    public Transform2D
{
public:
    OrthogonalCamera() noexcept;
    ~OrthogonalCamera();

    void init() noexcept;

    void setupProjectionMatrix(int32_t width, int32_t height) noexcept;
    void updateUniformBuffer(mat4 modelViewProjection) const noexcept;
    void flipVertically(bool flip) noexcept; // If false, the coordinates are counted in OpenGL space - (0, 0) in the lower left corner

    void getModelViewProjectionMatrix(mat4 mvp) noexcept;
    bool isUpsideDown() const noexcept;

private:
    alignas(16) mat4 m_projection;
    uint32_t m_uniformBuffer;

    bool m_modelViewNeedUpdate;
    bool m_flipVertically;
};

#endif // !ORTHOGONAL_CAMERA_HPP