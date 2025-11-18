#ifndef GL_RESOURCE_MANAGER_HPP
#define GL_RESOURCE_MANAGER_HPP

#include <span>
#include <string_view>
#include <unordered_map>

#include <glad/glad.h>

#include "resources/ogl/buffers/GlBuffer.hpp"
#include "resources/ogl/vao/VertexArrayObject.hpp"
#include "resources/ogl/texture/Texture.hpp"
#include "resources/ogl/shaders/ShaderProgram.hpp"


class GlResourceManager final
{
public:
    GlResourceManager() noexcept;
    ~GlResourceManager() noexcept;

	template<class T, size_t N>
    std::array<GLuint, N> create() noexcept;
    
    template<class T>
    void destroy(const std::span<GLuint> buffers) noexcept;

    GLuint getShaderProgram(std::string_view filename) noexcept;

private:
    template<size_t N>
    std::array<GLuint, N> createResources(std::vector<GLuint>& handles, void(*)(int32_t, GLuint*)) noexcept;
    
    void destroyResources(std::span<const GLuint> objects, std::vector<GLuint>& handles, void(*)(int32_t, const GLuint*)) noexcept;

    std::vector<GLuint> m_buffers;
    std::vector<GLuint> m_arrays;
    std::vector<GLuint> m_textures;
    std::unordered_map<std::string, ShaderProgram> m_programs;
};

#include "resources/ogl/holder/GlResourceManager.inl"

#endif // !GL_RESOURCE_MANAGER_HPP