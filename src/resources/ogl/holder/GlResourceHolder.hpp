#ifndef GL_RESOURCE_HOLDER_HPP
#define GL_RESOURCE_HOLDER_HPP

#include <span>

#include <glad/glad.h>

#include "resources/ogl/buffers/GlBuffer.hpp"
#include "resources/ogl/vao/VertexArrayObject.hpp"
#include "resources/ogl/texture/Texture.hpp"


class GlResourceHolder final
{
public:
    GlResourceHolder() noexcept;
    ~GlResourceHolder() noexcept;

	template<class T, size_t N>
    std::array<GLuint, N> create() noexcept;
    
    template<class T>
    void destroy(const std::span<GLuint> buffers) noexcept;

private:
    template<size_t N>
    std::array<GLuint, N> createResources(std::vector<GLuint>& handles, void(*)(int32_t, GLuint*)) noexcept;
    
    void destroyResources(std::span<const GLuint> objects, std::vector<GLuint>& handles, void(*)(int32_t, const GLuint*)) noexcept;

    std::vector<GLuint> m_buffers;
    std::vector<GLuint> m_arrays;
    std::vector<GLuint> m_textures;
};

#include "resources/ogl/holder/GlResourceHolder.inl"

#endif // !GL_RESOURCE_HOLDER_HPP