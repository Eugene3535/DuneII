#pragma once

#include <cstdint>
#include <span>
#include <string>

#include <entt/entity/fwd.hpp>

#include "graphics/transform/Transform2D.hpp"
#include "graphics/geometry/GraphicsData.hpp"
#include "scenes/mission/info/StructureInfo.hpp"


class Level final:
    public Transform2D
{
public:
    Level(struct Game* game) noexcept;
    ~Level();

    bool loadFromTileMap(const class TileMap& loader) noexcept;
    bool putStructure(const HouseType owner, const StructureInfo::Type type, const ivec2s cell) noexcept; // cell must be presented it tiles

    void draw(const mat4s& projection) const noexcept;

    entt::entity getEntityUnderCursor(const vec2s point) const noexcept;

private:
    bool createGraphicsResources(std::span<const vec4s> vertices, std::span<const uint32_t> indices) noexcept;
    void cleanupGraphicsResources() noexcept;

    void createGraphicsForEntity(const entt::entity entity) noexcept;
    void updateWall(int32_t origin, int32_t level)          noexcept;

    struct Game* m_game;

    std::string               m_tileMask;
    std::vector<entt::entity> m_structureMask;

    Landscape m_landscape;
    StructureStorage m_structures;

    ivec2s m_textureSize;
    ivec2s m_mapSize;
    ivec2s m_tileSize;
};
