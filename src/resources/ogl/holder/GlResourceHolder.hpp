#ifndef GL_RESOURCE_HOLDER_HPP
#define GL_RESOURCE_HOLDER_HPP

#include <span>
#include <vector>

#include <glad/glad.h>

class GlResourceHolder final
{
public:
    ~GlResourceHolder() noexcept;

    std::span<const GLuint> create(GLsizei amount, void(*func)(GLsizei, GLuint*))      noexcept;
    void destroy(std::span<const GLuint> buffers, void(*func)(GLsizei, const GLuint*)) noexcept;

private: // handles  
    std::vector<GLuint> m_buffers;
    std::vector<GLuint> m_arrays;
    std::vector<GLuint> m_textures;
};

#endif // !GL_RESOURCE_HOLDER_HPP