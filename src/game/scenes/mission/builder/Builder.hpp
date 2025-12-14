#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <unordered_map>

#include <entt/entity/registry.hpp>
#include <cglm/struct/ivec2.h>

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
    entt::registry& m_registry;
    std::unordered_map<int32_t, entt::entity> m_structureMap;
    uint32_t m_vertexBuffer;

    struct
    {
        Animation refinery;
        Animation starport;
        Animation turret;
        Animation rocketTurret;
        Animation wall;
        bool isLoaded;
    } m_animations;
    

    char* m_tileMask;
    int32_t m_mapWidth;
    int32_t m_mapHeight;
    int32_t m_tileWidth;
    int32_t m_tileHeight;
};

#endif // !BUILDER_HPP