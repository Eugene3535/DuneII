#include "resources/files/FileProvider.hpp"
#include "resources/files/Shader.hpp"
#include "resources/ogl/holder/GlResourceManager.hpp"


GlResourceManager::GlResourceManager() noexcept
{

}


GlResourceManager::~GlResourceManager() noexcept
{
    if(!m_buffers.empty())
        glDeleteBuffers(static_cast<GLsizei>(m_buffers.size()), m_buffers.data());

    if(!m_arrays.empty())
        glDeleteVertexArrays(static_cast<GLsizei>(m_arrays.size()), m_arrays.data());

    if(!m_textures.empty())
        glDeleteTextures(static_cast<GLsizei>(m_textures.size()), m_textures.data());
}


void GlResourceManager::destroyResources(std::span<const GLuint> objects, std::vector<GLuint>& handles, void(*func)(GLint, const GLuint*)) noexcept
{
    if(!objects.empty())
    {
//      Avoid deleting non-existent objects
        std::vector<GLuint> tmp;
        tmp.reserve(objects.size());

        for(GLuint object : objects)
        {
            for(size_t i = 0; i < handles.size(); ++i)
            {
                if(auto& handle = handles[i]; handle == object)
                {
                    tmp.push_back(object);
                    std::swap(handle, handles.back());
                    handles.pop_back();
                    break;
                }
            }
        }

        if(!tmp.empty())
            func(static_cast<GLint>(tmp.size()), tmp.data());
    }
}


GLuint GlResourceManager::getShaderProgram(std::string_view filename) noexcept
{
    if(auto it = m_programs.find(std::string(filename)); it != m_programs.end())
        return it->second.getHandle();

    struct ShaderInfo
    {
        size_t index;
        GLenum type;
    };

    auto get_shader_info = [](const std::string& extension) -> ShaderInfo
    {
        if(extension == ".vert")
            return { 0, GL_VERTEX_SHADER };

        if(extension == ".frag")
            return { 1, GL_FRAGMENT_SHADER };

        if(extension == ".geom")
            return { 2, GL_GEOMETRY_SHADER };

        return { SIZE_MAX, 0 };
    };

    FileProvider provider;
    auto shaderPaths = provider.findShaders(filename);

    if(shaderPaths.size() >= 2) // Minimum number of shaders
    {
        std::vector<Shader> shaders(shaderPaths.size());

        for(const auto& shaderPath : shaderPaths)
        {
            auto info = get_shader_info(shaderPath.extension().string());

            if(info.index == SIZE_MAX)
                return 0;

            if( ! shaders[info.index].loadFromFile(shaderPath, info.type) )
                return 0;
        }

        ShaderProgram program;
        volatile GLuint handle = 0;

        if(program.link(shaders))
        {
            handle = program.getHandle();
            m_programs[std::string(filename)] = std::move(program);
        }

        return handle;
    }

    return 0;
}