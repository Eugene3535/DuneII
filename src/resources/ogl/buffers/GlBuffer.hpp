#ifndef GL_BUFFER_HPP
#define GL_BUFFER_HPP

#include <cstdint>
#include <cstdlib>

#include <glad/glad.h>

// Class - buffer interface. 
// Targets:

// GL_ARRAY_BUFFER
// GL_ATOMIC_COUNTER_BUFFER
// GL_COPY_READ_BUFFER
// GL_COPY_WRITE_BUFFER
// GL_DISPATCH_INDIRECT_BUFFER
// GL_DRAW_INDIRECT_BUFFER
// GL_ELEMENT_ARRAY_BUFFER
// GL_PIXEL_PACK_BUFFER
// GL_PIXEL_UNPACK_BUFFER
// GL_QUERY_BUFFER
// GL_TEXTURE_BUFFER
// GL_SHADER_STORAGE_BUFFER
// GL_TRANSFORM_FEEDBACK_BUFFER
// GL_UNIFORM_BUFFER

// Usages: GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW

class GLBuffer final
{
public:
    GLBuffer() noexcept;
    GLBuffer(GLuint handle, GLenum target) noexcept;

    void create(size_t elementSize, size_t elementCount, const void* data, GLenum usage) noexcept;
    void update(size_t offset, size_t elementSize, size_t elementCount, const void* data) noexcept;

    void bindBufferRange(GLuint index, size_t offset, size_t size) const noexcept;

    GLuint getHandle() const noexcept;
    GLenum getTarget() const noexcept;
    GLuint getCount()  const noexcept;

private:
    GLuint m_handle;
    GLenum m_target;
    GLenum m_usage;
    GLuint m_count;
};

#endif // !GL_BUFFER_HPP