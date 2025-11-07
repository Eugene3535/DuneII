#include <cstring>

#include "resources/ogl/buffers/GlBuffer.hpp"


GLBuffer::GLBuffer() noexcept:
    m_handle(0),
    m_target(0),
    m_usage(GL_STREAM_DRAW),
    m_count(0)
{

}


GLBuffer::GLBuffer(GLuint handle, GLenum target) noexcept:
    m_handle(handle),
    m_target(target),
    m_usage(GL_STREAM_DRAW),
    m_count(0)
{
    
}


void GLBuffer::create(size_t elementSize, size_t elementCount, const void* data, GLenum usage) noexcept
{
    if(m_target)
    {
        glBindBuffer(m_target, m_handle);
        glBufferData(m_target, static_cast<GLsizeiptr>(elementCount * elementSize), data, usage);
        glBindBuffer(m_target, 0);
    
        m_usage = usage;
        m_count = static_cast<GLuint>(elementCount);
    }
}


void GLBuffer::update(size_t offset, size_t elementSize, size_t elementCount, const void* data) noexcept
{
    if(m_target)
    {
        glBindBuffer(m_target, m_handle);

        if(elementCount > m_count)
        {
            glBufferData(m_target, static_cast<GLsizeiptr>(elementCount * elementSize), nullptr, m_usage);
            m_count = static_cast<GLuint>(elementCount);
        }
            
        glBufferSubData(m_target, static_cast<GLintptr>(elementSize * offset), static_cast<GLsizeiptr>(elementCount * elementSize), data);
        glBindBuffer(m_target, 0);
    }
}


void GLBuffer::bindBufferRange(GLuint index, size_t offset, size_t size) const noexcept
{
    if(m_target)
    {
        //  target must be one of GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER, or GL_SHADER_STORAGE_BUFFER.
        const bool target_is_correct = ((m_target == GL_ATOMIC_COUNTER_BUFFER)     ||
                                        (m_target == GL_TRANSFORM_FEEDBACK_BUFFER) ||
                                        (m_target == GL_UNIFORM_BUFFER)            ||
                                        (m_target == GL_SHADER_STORAGE_BUFFER));                   
    
        if(target_is_correct)
        {
            glBindBuffer(m_target, m_handle);
            glBindBufferRange(m_target, index, m_handle, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size));
            glBindBuffer(m_target, 0);
        }
    }
}


GLuint GLBuffer::getHandle() const noexcept
{
    return m_handle;
}


GLenum GLBuffer::getTarget() const noexcept
{
    return m_target;
}


GLuint GLBuffer::getCount() const noexcept
{
    return m_count;
}