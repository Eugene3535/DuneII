#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <unordered_map>

#include <entt/entity/registry.hpp>

#include "graphics/sprites/SpriteManager.hpp"
#include "game/scenes/mission/common/Structures.hpp"


class Builder
{
public:
    Builder(entt::registry& registry) noexcept;
    ~Builder();

    void reset(const IConstructionSite& site) noexcept;

    bool loadFromTileMap(const class TileMap& tilemap) noexcept;
    bool putStructureOnMap(const Structure::Type type, const ivec2s cell) noexcept; // cell must be in tiles

    uint32_t getVertexBuffer() const noexcept;

private:
    void createGraphicsForEntity(const entt::entity entity) noexcept;
    void updateWall(int32_t origin, int32_t level) noexcept;

    entt::registry& m_registry;
    std::unordered_map<int32_t, entt::entity> m_structureMap;

    uint32_t m_vertexBuffer;
    void* m_mappedStorage;

    char* m_tileMask;
    ivec2s m_textureSize;
    ivec2s m_mapSize;
    ivec2s m_tileSize;
};

#endif // !BUILDER_HPP