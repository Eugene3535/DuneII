#include <cglm/struct/ivec4.h>

#include "game/scenes/mission/tilemap/TileMap.hpp"
#include "game/scenes/mission/builder/Builder.hpp"


enum class WallCellType : uint32_t
{
    DOT               = 1 << 1,
    LEFT_RIGHT        = 1 << 2,
    BOTTOM_TOP        = 1 << 3,
    TOP_RIGHT         = 1 << 4,
    RIGHT_BOTTOM      = 1 << 5,
    BOTTOM_LEFT       = 1 << 6,
    LEFT_TOP          = 1 << 7,
    TOP_RIGHT_BOTTOM  = 1 << 8,
    RIGHT_BOTTOM_LEFT = 1 << 9,
    BOTTOM_LEFT_TOP   = 1 << 10,
    LEFT_TOP_RIGHT    = 1 << 11,
    CROSS             = 1 << 12
};


static WallCellType compute_wall_type(bool left, bool top, bool right, bool bottom)     noexcept;
static ivec4s       get_texcoords_of_custom_wall(WallCellType type)                     noexcept;
static ivec4s       get_texcoords_of_structure(Structure::Type type)                    noexcept;
static ivec4s       get_bounds_of(Structure::Type type, int32_t coordX, int32_t coordY) noexcept; // in tiles
static int32_t      get_armor_of(Structure::Type type)                                  noexcept;



Builder::Builder(entt::registry& registry) noexcept:
    m_registry(registry),
    m_tileMask(nullptr),
    m_width(0),
    m_height(0)
{

}


void Builder::setup(const IConstructionSite& site) noexcept
{
    m_tileMask = site.tileMask;
    m_width    = site.width;
    m_height   = site.height;
}


bool Builder::loadFromTileMap(const TileMap& tilemap) noexcept
{
    return false;
}


bool Builder::putStructureOnMap(Structure::Type type, ivec2s cell) noexcept
{
	if(type >= Structure::Type::MAX)
        return false;

    return false;
}


WallCellType compute_wall_type(bool left, bool top, bool right, bool bottom) noexcept
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


ivec4s get_texcoords_of_custom_wall(WallCellType type) noexcept
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


ivec4s get_texcoords_of_structure(Structure::Type type) noexcept
{
	switch (type)
	{
		case Structure::Type::SLAB_1x1:          return { 0,   160, 32, 32 };
		case Structure::Type::PALACE:            return { 64,  96 , 96, 96 };
		case Structure::Type::VEHICLE:           return { 256, 32 , 96, 64 };
		case Structure::Type::HIGH_TECH:         return { 160, 96 , 64, 64 };
		case Structure::Type::CONSTRUCTION_YARD: return { 0,   32 , 64, 64 };
		case Structure::Type::WIND_TRAP:         return { 64,  32 , 64, 64 };
		case Structure::Type::BARRACKS:          return { 0,   96 , 64, 64 };
		case Structure::Type::STARPORT:          return { 0,   192, 96, 96 };
		case Structure::Type::REFINERY:          return { 416, 0  , 96, 64 };
		case Structure::Type::REPAIR:            return { 224, 96 , 96, 64 };
		case Structure::Type::WALL:              return { 0,   0  , 32, 32 };
		case Structure::Type::TURRET:            return { 192, 288, 32, 32 };
		case Structure::Type::ROCKET_TURRET:     return { 448, 288, 32, 32 };
		case Structure::Type::SILO:              return { 192, 32 , 64, 64 };
		case Structure::Type::OUTPOST:           return { 128, 32 , 64, 64 };

		default: return { 0, 0, 0, 0 };
	}
}


ivec4s get_bounds_of(Structure::Type type, int32_t coordX, int32_t coordY) noexcept
{
	switch (type)
	{
		case Structure::Type::SLAB_1x1:          return { coordX, coordY, 1, 1 };
		case Structure::Type::PALACE:            return { coordX, coordY, 3, 3 };
		case Structure::Type::VEHICLE:           return { coordX, coordY, 3, 2 };
		case Structure::Type::HIGH_TECH:         return { coordX, coordY, 2, 2 };
		case Structure::Type::CONSTRUCTION_YARD: return { coordX, coordY, 2, 2 };
		case Structure::Type::WIND_TRAP:         return { coordX, coordY, 2, 2 };
		case Structure::Type::BARRACKS:          return { coordX, coordY, 2, 2 };
		case Structure::Type::STARPORT:          return { coordX, coordY, 3, 3 };
		case Structure::Type::REFINERY:          return { coordX, coordY, 3, 2 };
		case Structure::Type::REPAIR:            return { coordX, coordY, 3, 2 };
		case Structure::Type::WALL:              return { coordX, coordY, 1, 1 };
		case Structure::Type::TURRET:            return { coordX, coordY, 1, 1 };
		case Structure::Type::ROCKET_TURRET:     return { coordX, coordY, 1, 1 };
		case Structure::Type::SILO:              return { coordX, coordY, 2, 2 };
		case Structure::Type::OUTPOST:           return { coordX, coordY, 2, 2 }; 

		default: return { 0, 0, 0, 0 };
	}
}


int32_t get_armor_of(Structure::Type type) noexcept
{
	switch (type)
	{
		case Structure::Type::SLAB_1x1:          return 40;
		case Structure::Type::PALACE:            return 2000;
		case Structure::Type::VEHICLE:           return 800;
		case Structure::Type::HIGH_TECH:         return 1000;
		case Structure::Type::CONSTRUCTION_YARD: return 800;
		case Structure::Type::WIND_TRAP:         return 400;
		case Structure::Type::BARRACKS:          return 600;
		case Structure::Type::STARPORT:          return 1000;
		case Structure::Type::REFINERY:          return 900;
		case Structure::Type::REPAIR:            return 1800;
		case Structure::Type::WALL:              return 140;
		case Structure::Type::TURRET:            return 025;
		case Structure::Type::ROCKET_TURRET:     return 500;
		case Structure::Type::SILO:              return 300;
		case Structure::Type::OUTPOST:           return 1000; 

		default: return 1;
	}
}