#ifndef VERTEX_ARRAY_OBJECT_HPP
#define VERTEX_ARRAY_OBJECT_HPP

#include "resources/ogl/buffers/GlBuffer.hpp"
#include "resources/ogl/vao/attributes/VertexBufferLayout.hpp"

class VertexArrayObject final
{
public:
    VertexArrayObject() noexcept;
    VertexArrayObject(GLuint handle) noexcept;

    void addVertexBuffer(const GLBuffer& buffer, std::span<const VertexBufferLayout::Attribute> attributes) noexcept;
    void setElementBuffer(const GLBuffer& buffer) noexcept;

    GLuint getHandle()     const noexcept;
    GLuint getIndexCount() const noexcept;

private:
    GLuint m_handle;
    GLuint m_attributeCount;
    GLuint m_indexCount;
};

#endif // !VERTEX_ARRAY_OBJECT_HPP