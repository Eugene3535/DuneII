#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <unordered_map>
#include <optional>

#include <entt/entity/registry.hpp>

#include "game/scenes/mission/common/Structures.hpp"


class Builder
{
public:
    Builder(entt::registry& registry, std::string& tileMask) noexcept;
    ~Builder();

    bool loadFromTileMap(const class TileMap& tilemap, const uint32_t texture) noexcept;
    bool putStructureOnMap(const Structure::Type type, const ivec2s cell)      noexcept; // cell must be in tiles

    uint32_t                    getVertexBuffer()                       const noexcept;
    std::optional<entt::entity> getEntityUnderCursor(const vec2s point) const noexcept;
    entt::registry& getRegistry()                                       const noexcept;

private:
    void initStorage()                                      noexcept;
    void createGraphicsForEntity(const entt::entity entity) noexcept;
    void updateWall(int32_t origin, int32_t level)          noexcept;

    entt::registry&                           m_registry;
    std::string&                              m_tileMask;
    std::unordered_map<int32_t, entt::entity> m_structureMap;

    uint32_t m_vertexBuffer;
    void*    m_mappedStorage;

    ivec2s m_textureSize;
    ivec2s m_mapSize;
    ivec2s m_tileSize;
};

#endif // !BUILDER_HPP