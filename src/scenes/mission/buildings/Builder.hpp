#ifndef WALL_BUILDER_HPP
#define WALL_BUILDER_HPP

#include "scenes/mission/tilemap/TileMap.hpp"

#include <SFML/Graphics/Rect.hpp>

class Builder
{
    enum WallCellType : uint32_t
    {
        DOT = 1 << 1,
        LEFT_RIGHT = 1 << 2,
        BOTTOM_TOP = 1 << 3,
        TOP_RIGHT = 1 << 4,
        RIGHT_BOTTOM = 1 << 5,
        BOTTOM_LEFT = 1 << 6,
        LEFT_TOP = 1 << 7,
        TOP_RIGHT_BOTTOM = 1 << 8,
        RIGHT_BOTTOM_LEFT = 1 << 9,
        BOTTOM_LEFT_TOP = 1 << 10,
        LEFT_TOP_RIGHT = 1 << 11,
        CROSS = 1 << 12
    };

public:
    Builder() noexcept;
    ~Builder();

    bool init(TileMap& tilemap) noexcept;

    void placeWall(int32_t cellX, int32_t cellY) noexcept;

private:
    void         updateWall(int32_t origin, int32_t level) noexcept;
    WallCellType getWallType(bool left, bool top, bool right, bool bottom) noexcept;
    sf::IntRect  getTexCoordsOf(WallCellType type) noexcept;

private:
    std::unordered_map<int32_t, Building>* m_buildings;
    std::string* m_tileMask;
    int32_t m_mapWidthInTiles;
    int32_t m_mapHeightInTiles;
    int32_t m_tileWidth;
    int32_t m_tileHeight;
};

#endif // !WALL_BUILDER_HPP