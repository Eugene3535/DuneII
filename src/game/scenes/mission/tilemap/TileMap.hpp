#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <entt/entity/registry.hpp>

#include "graphics/transform/Transform2D.hpp"
#include "game/scenes/mission/common/Structures.hpp"


class Tilemap final:
    public Transform2D
{
public:
    Tilemap(class Engine* engine, entt::registry& registry) noexcept;
    ~Tilemap();

    bool createFromLoader(const class TiledMapLoader& loader) noexcept;
    bool putStructure(const HouseType owner, const StructureInfo::Type type, const ivec2s cell)  noexcept; // cell must be in tiles

    void draw() const noexcept;

    entt::entity    getEntityUnderCursor(const vec2s point) const noexcept;
    entt::registry& getRegistry()                           const noexcept;

private:
    bool createGraphicsResources(std::span<const vec4s> vertices, std::span<const uint32_t> indices) noexcept;
    void cleanupGraphicsResources() noexcept;

    void createGraphicsForEntity(const entt::entity entity) noexcept;
    void updateWall(int32_t origin, int32_t level)          noexcept;

    Engine*         m_engine;
    entt::registry& m_registry;

    std::string               m_tileMask;
    std::vector<entt::entity> m_structureMask;

    struct
	{
		uint32_t texture;
		uint32_t vertexArrayObject;
		uint32_t vertexBufferObjects[2];
		uint32_t count; // indices
        void*    mappedStorage;
		uint32_t program;
	} m_landscape;

    struct
	{
        uint32_t texture;
        uint32_t vertexBufferObject;
        uint32_t vertexArrayObject;
        void*    mappedStorage;
	} m_buildings;

    ivec2s m_textureSize;
    ivec2s m_mapSize;
    ivec2s m_tileSize;
};

#endif // !TILEMAP_HPP