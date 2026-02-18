#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <entt/entity/registry.hpp>

#include "game/scenes/mission/common/Structures.hpp"


class Tilemap
{
public:
    Tilemap(entt::registry& registry, const class Engine* engine) noexcept;
    ~Tilemap();

    bool createFromLoader(const class TiledMapLoader& loader) noexcept;
    bool putStructure(const HouseType owner, const StructureInfo::Type type, const ivec2s cell)  noexcept; // cell must be in tiles

    void draw() const noexcept;

    uint32_t        getVertexBuffer()                       const noexcept;
    entt::entity    getEntityUnderCursor(const vec2s point) const noexcept;
    entt::registry& getRegistry()                           const noexcept;

private:
    bool createGraphicsResources(std::span<const vec4s> vertices, std::span<const uint32_t> indices) noexcept;
    void cleanupGraphicsResources() noexcept;

    void createGraphicsForEntity(const entt::entity entity) noexcept;
    void updateWall(int32_t origin, int32_t level)          noexcept;

    entt::registry&           m_registry;
    std::string               m_tileMask;
    const class Engine*       m_engine;
    std::vector<entt::entity> m_structureMask;

    uint32_t m_vertexBuffer;
    void*    m_mappedStorage;

    struct
	{
		uint32_t texture;
		uint32_t vao;
		uint32_t vbo[2];
		uint32_t count; // indices
		uint32_t program;
	} m_landscape;

    struct
	{
		uint32_t texture;
		uint32_t vao;
	} m_buildings;

    ivec2s m_textureSize;
    ivec2s m_mapSize;
    ivec2s m_tileSize;
};

#endif // !TILEMAP_HPP