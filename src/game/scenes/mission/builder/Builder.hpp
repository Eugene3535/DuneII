#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <entt/entity/registry.hpp>
#include <cglm/struct/ivec2.h>

#include "game/scenes/mission/common/Structures.hpp"


class Builder
{
public:
    Builder(entt::registry& registry) noexcept;

    void setup(const IConstructionSite& site) noexcept;

    bool loadFromTileMap(const class TileMap& tilemap) noexcept;
    bool putStructureOnMap(Structure::Type type, ivec2s cell) noexcept;

private:
    entt::registry& m_registry;
    char* m_tileMask;
    int32_t m_width;
    int32_t m_height;
};

#endif // !BUILDER_HPP