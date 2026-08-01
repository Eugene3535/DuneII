#pragma once

#include <vector>
#include <span>
#include <unordered_map>
#include <string>

class GlResourceManager
{
public:
    enum Resource: uint32_t
    {
        GLTexture2D,
        GLBuffer,
        GLVertexArray
    };

    GlResourceManager() noexcept;
    ~GlResourceManager();

    std::span<const uint32_t> getHandles(Resource type, uint32_t count) noexcept;
    void destroyHandles(Resource type, std::span<const uint32_t> handles) noexcept;

    void appendPrograms(std::unordered_map<std::string, uint32_t>& programs) noexcept;
    uint32_t getProgram(const std::string& name) const noexcept; 

    void reset() noexcept;

private:
    std::vector<uint32_t> m_textures;
    std::vector<uint32_t> m_buffers;
    std::vector<uint32_t> m_vertexArrays;
    std::unordered_map<std::string, uint32_t> m_shaderPrograms;
};