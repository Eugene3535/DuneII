#include <glad/glad.h>
#include <cglm/struct/ivec2.h>
#include <cglm/struct/ivec4.h>

#include "game/scenes/mission/tilemap/TileMap.hpp"
#include "game/scenes/mission/builder/Builder.hpp"


#define STRUCTURE_LIMIT_ON_MAP 256

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


static WallCellType compute_wall_type(bool left, bool top, bool right, bool bottom) noexcept;
static ivec4s       get_texcoords_of_custom_wall(const WallCellType type)           noexcept;
static ivec4s       get_texcoords_of_structure(const Structure::Type type)          noexcept;
static ivec4s       get_bounds_of(const Structure::Type type, const ivec2s cell)    noexcept; // cell value must be presented in tiles, not pixels
static int32_t      get_armor_of(const Structure::Type type)                        noexcept;



Builder::Builder(entt::registry& registry) noexcept:
    m_registry(registry),
	m_vertexBuffer(0),
    m_tileMask(nullptr),
    m_mapWidth(0),
    m_mapHeight(0),
	m_tileWidth(0),
    m_tileHeight(0)
{
	m_animations.isLoaded = false;

	glCreateBuffers(1, &m_vertexBuffer);
	glNamedBufferData(m_vertexBuffer, STRUCTURE_LIMIT_ON_MAP * sizeof(float) * 16, nullptr, GL_DYNAMIC_DRAW);
}


Builder::~Builder()
{
	glDeleteBuffers(1, &m_vertexBuffer);
}


void Builder::reset(const IConstructionSite& site) noexcept
{
	m_structureMap.clear();
    m_tileMask  = site.tileMask;
    m_mapWidth  = site.mapWidth;
    m_mapHeight = site.mapHeight;
	m_tileWidth  = site.tileWidth;
    m_tileHeight = site.tileHeight;
}


bool Builder::loadFromTileMap(const TileMap& tilemap) noexcept
{
    return false;
}


bool Builder::putStructureOnMap(const Structure::Type type, const ivec2s cell) noexcept
{
	if(!m_tileMask)
		return false;

	if(type >= Structure::Type::MAX)
        return false;

	int32_t coordX = cell.x * m_tileWidth;
	int32_t coordY = cell.y * m_tileHeight;
    auto bounds = get_bounds_of(type, cell);

    {// out of bounds (in pixels) ?
        const int32_t mapWidth  = m_mapWidth * m_tileWidth;
        const int32_t mapHeight = m_mapHeight * m_tileHeight;

        if(bounds.x < 0)         return false;
        if(bounds.y < 0)         return false;
        if(bounds.z > mapWidth)  return false;
        if(bounds.w > mapHeight) return false;
    }

    const int32_t origin = cell.y * m_mapWidth + cell.x;

	if(auto found = m_structureMap.find(origin); found != m_structureMap.end())
		return false;

    {
        ivec2s size;

        switch (type)
        {
            case Structure::Type::SLAB_1x1:          size = { 1, 1 }; break;
			case Structure::Type::PALACE:            size = { 3, 3 }; break;
			case Structure::Type::VEHICLE:           size = { 3, 2 }; break;
			case Structure::Type::HIGH_TECH:         size = { 2, 2 }; break;
            case Structure::Type::CONSTRUCTION_YARD: size = { 2, 2 }; break;
			case Structure::Type::WIND_TRAP:         size = { 2, 2 }; break;
            case Structure::Type::BARRACKS:          size = { 2, 2 }; break;
			case Structure::Type::STARPORT:          size = { 3, 3 }; break;
			case Structure::Type::REFINERY:          size = { 3, 2 }; break;
			case Structure::Type::REPAIR:            size = { 3, 2 }; break;
            case Structure::Type::WALL:              size = { 1, 1 }; break;
            case Structure::Type::TURRET:            size = { 1, 1 }; break;
            case Structure::Type::ROCKET_TURRET:     size = { 1, 1 }; break;
            case Structure::Type::SILO:              size = { 2, 2 }; break;
            case Structure::Type::OUTPOST:           size = { 2, 2 }; break;

            default: break;
        }

        int32_t offset = origin;

        for (int32_t i = 0; i < size.y; ++i)
        {
            for (int32_t j = 0; j < size.x; ++j)  
                if(m_tileMask[offset + j] != 'R')
                    return false;
            
            offset += m_mapWidth;
        }
    }

	const auto entity = m_registry.create();
	m_registry.emplace<ivec4s>(entity, bounds);
	auto& structure = m_registry.emplace<Structure>(entity);

	
	// auto& tile = m_registry.emplace<Tile>(entity, texture, getTexCoordsOf(type));
	
	// structure.type = type;
	// tile.setPosition(sf::Vector2f(coordX, coordY));
	// structure.hitPoints = structure.maxHitPoints = getHitPointsOf(type);

	// auto setup_tiles_on_mask = [this, origin, entity](int32_t width, int32_t height, char symbol = 'B') -> void
	// {
	// 	int32_t offset = origin;

	// 	for (int32_t i = 0; i < height; ++i)
	// 	{
	// 		for (int32_t j = 0; j < width; ++j)
	// 		{
	// 			m_tileMask[offset + j] = symbol;
	// 			m_structureMap[offset + j] = entity;
	// 		}

	// 		offset += m_mapWidth;
	// 	}
	// };

	// switch(type)
	// {
	// 	case Structure::Type::SLAB_1x1:          setup_tiles_on_mask(1, 1, 'C'); break;
	// 	case Structure::Type::PALACE:            setup_tiles_on_mask(3, 3);      break;
	// 	case Structure::Type::VEHICLE:           setup_tiles_on_mask(3, 2);      break;
	// 	case Structure::Type::HIGH_TECH:         setup_tiles_on_mask(2, 2);      break;
	// 	case Structure::Type::CONSTRUCTION_YARD: setup_tiles_on_mask(2, 2);      break;
	// 	case Structure::Type::WIND_TRAP:         setup_tiles_on_mask(2, 2);      break;
	// 	case Structure::Type::BARRACKS:          setup_tiles_on_mask(2, 2);      break;
	// 	case Structure::Type::STARPORT:          setup_tiles_on_mask(3, 3);      break;
	// 	case Structure::Type::REFINERY:          setup_tiles_on_mask(3, 2);      break;
	// 	case Structure::Type::REPAIR:            setup_tiles_on_mask(3, 2);      break;
	// 	case Structure::Type::WALL:              setup_tiles_on_mask(1, 1, 'W'); break;
	// 	case Structure::Type::TURRET:            setup_tiles_on_mask(1, 1);      break;
	// 	case Structure::Type::ROCKET_TURRET:     setup_tiles_on_mask(1, 1);      break;
	// 	case Structure::Type::SILO:              setup_tiles_on_mask(2, 2);      break;
	// 	case Structure::Type::OUTPOST:           setup_tiles_on_mask(2, 2);      break;

	// 	default: break;
	// }

	// if(type == StructureType::WALL)
	// 	updateWall(origin, 2);

	return true;
}


uint32_t Builder::getVertexBuffer() const noexcept
{
	return m_vertexBuffer;
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


ivec4s get_texcoords_of_custom_wall(const WallCellType type) noexcept
{
    switch (type)
    {
		default:
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
    }
}


ivec4s get_texcoords_of_structure(const Structure::Type type) noexcept
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


ivec4s get_bounds_of(const Structure::Type type, const ivec2s cell) noexcept
{
	switch (type)
	{
		case Structure::Type::SLAB_1x1:          return { cell.x, cell.y, cell.x + 32, cell.y + 32 };
		case Structure::Type::PALACE:            return { cell.x, cell.y, cell.x + 96, cell.y + 96 };
		case Structure::Type::VEHICLE:           return { cell.x, cell.y, cell.x + 96, cell.y + 64 };
		case Structure::Type::HIGH_TECH:         return { cell.x, cell.y, cell.x + 64, cell.y + 64 };
		case Structure::Type::CONSTRUCTION_YARD: return { cell.x, cell.y, cell.x + 64, cell.y + 64 };
		case Structure::Type::WIND_TRAP:         return { cell.x, cell.y, cell.x + 64, cell.y + 64 };
		case Structure::Type::BARRACKS:          return { cell.x, cell.y, cell.x + 64, cell.y + 64 };
		case Structure::Type::STARPORT:          return { cell.x, cell.y, cell.x + 96, cell.y + 96 };
		case Structure::Type::REFINERY:          return { cell.x, cell.y, cell.x + 96, cell.y + 64 };
		case Structure::Type::REPAIR:            return { cell.x, cell.y, cell.x + 96, cell.y + 64 };
		case Structure::Type::WALL:              return { cell.x, cell.y, cell.x + 32, cell.y + 32 };
		case Structure::Type::TURRET:            return { cell.x, cell.y, cell.x + 32, cell.y + 32 };
		case Structure::Type::ROCKET_TURRET:     return { cell.x, cell.y, cell.x + 32, cell.y + 32 };
		case Structure::Type::SILO:              return { cell.x, cell.y, cell.x + 64, cell.y + 64 };
		case Structure::Type::OUTPOST:           return { cell.x, cell.y, cell.x + 64, cell.y + 64 }; 

		default: return { 0, 0, 0, 0 };
	}
}


int32_t get_armor_of(const Structure::Type type) noexcept
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