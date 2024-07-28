#include "scenes/mission/buildings/Builder.hpp"

Builder::Builder() noexcept:
    m_buildings(nullptr),
    m_tileMask(nullptr),
    m_mapWidthInTiles(0),
    m_mapHeightInTiles(0),
    m_tileWidth(0),
    m_tileHeight(0)
{

}

Builder::~Builder()
{

}

bool Builder::init(TileMap& tilemap) noexcept
{
    m_buildings        = &tilemap.m_buildings;
    m_tileMask         = &tilemap.tileMask;
    m_mapWidthInTiles  = tilemap.mapSizeInTiles.x;
    m_mapHeightInTiles = tilemap.mapSizeInTiles.y;
    m_tileWidth        = tilemap.tileSize.x;
    m_tileHeight       = tilemap.tileSize.y;

    for(auto& [id, building] : *m_buildings)
    {
        if(building.getType() == Building::WALL)
        {
            updateWall(id, 1);
        }
    }

    return true;
}

void Builder::placeWall(int32_t cellX, int32_t cellY) noexcept
{
    int32_t origin = cellY * m_mapWidthInTiles + cellX;
    auto wall_cell = m_tileMask->data() + origin;
    *wall_cell = 'W';
    updateWall(origin, 2);
}

void Builder::updateWall(int32_t origin, int32_t level) noexcept
{
    if(level < 1) return;

    int32_t left   = origin - 1;
    int32_t top    = origin - m_mapWidthInTiles;
    int32_t right  = origin + 1;
    int32_t bottom = origin + m_mapWidthInTiles;

    const char* field = m_tileMask->data();

    bool a = (left >= 0)                   ? (field[left]   == 'W') : false;
    bool b = (top >= 0)                    ? (field[top]    == 'W') : false;
    bool c = (right < m_tileMask->size())  ? (field[right]  == 'W') : false;
    bool d = (bottom < m_tileMask->size()) ? (field[bottom] == 'W') : false;

    auto wall_type  = getWallType(a, b, c, d);
    auto tex_coords = getTexCoordsOf(wall_type);

    if(auto pair = m_buildings->find(origin); pair != m_buildings->end())
    {
        auto& sprite = pair->second;
        sprite.setTextureRect(tex_coords);
    }

    if(a) updateWall(left, level - 1);
    if(b) updateWall(top, level - 1);
    if(c) updateWall(right, level - 1);
    if(d) updateWall(bottom, level - 1);
}

Builder::WallCellType Builder::getWallType(bool left, bool top, bool right, bool bottom) noexcept
{
    if(!left && !top && !right && !bottom) return WallCellType::DOT;
    if((left || right) && !top && !bottom) return WallCellType::LEFT_RIGHT;
    if(!left && !right && (top || bottom)) return WallCellType::BOTTOM_TOP;
    if(top && right && !left && !bottom)   return WallCellType::TOP_RIGHT;
    if(!left && !top && right && bottom)   return WallCellType::RIGHT_BOTTOM;
    if(left && bottom && !top && !right)   return WallCellType::BOTTOM_LEFT;
    if(left && top && !right && !bottom)   return WallCellType::LEFT_TOP;
    if(!left && top && right && bottom)    return WallCellType::TOP_RIGHT_BOTTOM;
    if(left && right && bottom && !top)    return WallCellType::RIGHT_BOTTOM_LEFT;
    if(left && top && bottom && !right)    return WallCellType::BOTTOM_LEFT_TOP;
    if(left && top && right && !bottom)    return WallCellType::LEFT_TOP_RIGHT;

    return WallCellType::CROSS;
}

sf::IntRect Builder::getTexCoordsOf(Builder::WallCellType type) noexcept
{
    switch (type)
    {
        case WallCellType::DOT:               return { 0,   0, 32, 32 }; 
        case WallCellType::LEFT_RIGHT:        return { 32,  0, 32, 32 }; 
        case WallCellType::BOTTOM_TOP:        return { 64,  0, 32, 32 }; 
        case WallCellType::TOP_RIGHT:         return { 96,  0, 32, 32 }; 
        case WallCellType::RIGHT_BOTTOM:      return { 128, 0, 32, 32 };
        case WallCellType::BOTTOM_LEFT:       return { 160, 0, 32, 32 };
        case WallCellType::LEFT_TOP:          return { 192, 0, 32, 32 };
        case WallCellType::TOP_RIGHT_BOTTOM:  return { 224, 0, 32, 32 }; 
        case WallCellType::RIGHT_BOTTOM_LEFT: return { 256, 0, 32, 32 }; 
        case WallCellType::BOTTOM_LEFT_TOP:   return { 288, 0, 32, 32 };
        case WallCellType::LEFT_TOP_RIGHT:    return { 320, 0, 32, 32 };
        case WallCellType::CROSS:             return { 352, 0, 32, 32 };
    
        default: return { 0, 0, 32, 32 };
    }
}