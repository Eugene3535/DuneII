#include <glad/glad.h>

#include "graphics/resources/GlResourceManager.hpp"


GlResourceManager::GlResourceManager() noexcept = default;


GlResourceManager::~GlResourceManager()
{
    reset();
}


std::span<const uint32_t> GlResourceManager::getHandles(Resource type, uint32_t count) noexcept
{
    std::vector<uint32_t> handles(count);
    std::vector<uint32_t>* container { nullptr };

    switch (type)
    {
        case Resource::GLTexture2D:
            glCreateTextures(GL_TEXTURE_2D, static_cast<GLsizei>(count), handles.data());
            container = &m_textures;
        break;

        case Resource::GLBuffer:
            glCreateBuffers(static_cast<GLsizei>(count), handles.data());
            container = &m_buffers;
        break;

        case Resource::GLVertexArray:
            glCreateVertexArrays(static_cast<GLsizei>(count), handles.data());
            container = &m_vertexArrays;
        break;
        
        default:
            break;
    }

    if (container)
    {
        container->insert(container->end(), handles.begin(), handles.end());
        const size_t index = container->size() - count;
        std::span<const uint32_t> sp(container->begin() + index, count);

        return sp;
    }

    return {};
}


void GlResourceManager::destroyHandles(Resource type, std::span<const uint32_t> handles) noexcept
{
    std::vector<uint32_t>* container { nullptr };

    if (!handles.empty())
    {
        switch (type)
        {
            case Resource::GLTexture2D:
                glDeleteTextures(static_cast<GLsizei>(handles.size()), handles.data());
                container = &m_textures;
            break;

            case Resource::GLBuffer:
                glDeleteBuffers(static_cast<GLsizei>(handles.size()), handles.data());
                container = &m_buffers;
            break;

            case Resource::GLVertexArray:
                glDeleteVertexArrays(static_cast<GLsizei>(handles.size()), handles.data());
                container = &m_vertexArrays;
            break;
            
            default:
                break;
        }

        if (container)
        {
            for (const auto handle : handles)
            {
                for (size_t i = 0; i < container->size(); ++i)
                {
                    if (handle == (*container)[i])
                    {
                        std::swap((*container)[i], container->back());
                        container->pop_back();
                    }
                }
            }
        }
    }
}


void GlResourceManager::appendPrograms(std::unordered_map<std::string, uint32_t>& programs) noexcept
{
    m_shaderPrograms.swap(programs);
}


uint32_t GlResourceManager::getProgram(const std::string& name) const noexcept
{
    if (auto it = m_shaderPrograms.find(name); it != m_shaderPrograms.end())
        return it->second;

    return 0;
}


void GlResourceManager::reset() noexcept
{
    if (!m_textures.empty()) 
    {
        glDeleteTextures(static_cast<GLsizei>(m_textures.size()), m_textures.data());
        m_textures.clear();
    }

    if (!m_buffers.empty()) 
    {
        glDeleteBuffers(static_cast<GLsizei>(m_buffers.size()), m_buffers.data());
        m_buffers.clear();
    }

    if (!m_vertexArrays.empty()) 
    {
        glDeleteVertexArrays(static_cast<GLsizei>(m_vertexArrays.size()), m_vertexArrays.data());
        m_vertexArrays.clear();
    }

    if (!m_shaderPrograms.empty()) 
    {
        for (const auto& [_, program] : m_shaderPrograms)
            glDeleteProgram(program);

        m_shaderPrograms.clear();
    }
}