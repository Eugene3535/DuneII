#include <glad/glad.h>
#include <cglm/struct/ivec2.h>

#include "resources/gl_interfaces/texture/Texture.hpp"
#include "game/scenes/mission/tilemap/TileMap.hpp"
#include "game/scenes/mission/builder/Builder.hpp"


#define STRUCTURE_LIMIT_ON_MAP 512
#define BUILDING_LIMIT_ON_MAP 128


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


static WallCellType compute_wall_type(bool left, bool top, bool right, bool bottom)                     noexcept;
static vec4s        get_texcoords_of_custom_wall(const WallCellType type, ivec2s textureSize)           noexcept;
static vec4s        get_texcoords_of_structure(const Structure::Type type, float width, float height)   noexcept;
static ivec4s       get_bounds_of(const Structure::Type type, const ivec2s cell, const ivec2s tileSize) noexcept; // cell value must be presented in tiles, not pixels
static int32_t      get_armor_of(const Structure::Type type)                                            noexcept;



Builder::Builder(entt::registry& registry, std::string& tileMask) noexcept:
    m_registry(registry),
	m_tileMask(tileMask),
	m_vertexBuffer(0),
	m_mappedStorage(nullptr),
	m_textureSize(glms_ivec2_zero()),
	m_mapSize(glms_ivec2_zero()),
	m_tileSize(glms_ivec2_zero())
{
	glCreateBuffers(1, &m_vertexBuffer);

	glNamedBufferStorage(
		m_vertexBuffer,
		STRUCTURE_LIMIT_ON_MAP * sizeof(vec4s) * 4,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT |
		GL_MAP_WRITE_BIT | 
		GL_MAP_PERSISTENT_BIT | 
		GL_MAP_COHERENT_BIT
	);

	m_mappedStorage = glMapNamedBufferRange(
		m_vertexBuffer,
		0, 
		STRUCTURE_LIMIT_ON_MAP * sizeof(vec4s) * 4,
		GL_MAP_WRITE_BIT |
		GL_MAP_PERSISTENT_BIT |
		GL_MAP_COHERENT_BIT |
		GL_MAP_UNSYNCHRONIZED_BIT
	);
}


Builder::~Builder()
{
	if(m_mappedStorage)
		glUnmapNamedBuffer(m_vertexBuffer);
		
    glDeleteBuffers(1, &m_vertexBuffer);
}


bool Builder::loadFromTileMap(const TileMap& tilemap, const Texture& texture) noexcept
{
	m_structureMap.clear();
	m_mapSize = tilemap.getMapSize();
	m_tileSize = tilemap.getTileSize();
	m_textureSize = { texture.width, texture.height };

	auto get_structure_enum = [](const std::string& name) -> Structure::Type
	{
		if(name == "Wall")          return Structure::Type::WALL;
		if(name == "Refinery")      return Structure::Type::REFINERY;
		if(name == "ConstructYard") return Structure::Type::CONSTRUCTION_YARD ;
		if(name == "WindTrap")      return Structure::Type::WIND_TRAP;
		if(name == "Outpost")       return Structure::Type::OUTPOST;
		if(name == "Silo")          return Structure::Type::SILO;
		if(name == "Vehicle")       return Structure::Type::VEHICLE;
		if(name == "Barracks")      return Structure::Type::BARRACKS;
		if(name == "Palace")        return Structure::Type::PALACE;
		if(name == "HighTech")      return Structure::Type::HIGH_TECH;
		if(name == "Repair")        return Structure::Type::REPAIR;
		if(name == "Slab_1x1")      return Structure::Type::SLAB_1x1;
		if(name == "Starport")      return Structure::Type::STARPORT;
		if(name == "Turret")        return Structure::Type::TURRET;
		if(name == "RocketTurret")  return Structure::Type::ROCKET_TURRET;

		return Structure::Type::INVALID;
	};

	auto objects = tilemap.getObjects();

	if(objects.empty())
		return false;

	for(const auto& object : objects)
	{
		if (object.type == "Structure")
		{
			const auto type = get_structure_enum(object.name);
			const ivec2s cell = { object.bounds.x, object.bounds.y };
			putStructureOnMap(type, cell);
		}
	}

    return true;
}


bool Builder::putStructureOnMap(const Structure::Type type, const ivec2s cell) noexcept
{
	if(auto size = m_registry.storage<Structure>().size(); size >= STRUCTURE_LIMIT_ON_MAP)
		return false;

	if(type >= Structure::Type::MAX)
        return false;

    auto bounds = get_bounds_of(type, cell, m_tileSize);

    {// out of bounds (in pixels) ?
        const int32_t mapWidth  = m_mapSize.x * m_tileSize.x;
        const int32_t mapHeight = m_mapSize.y * m_tileSize.y;

        if(bounds.x < 0)         return false;
        if(bounds.y < 0)         return false;
        if(bounds.z > mapWidth)  return false;
        if(bounds.w > mapHeight) return false;
    }

    const int32_t origin = cell.y * m_mapSize.x + cell.x;

	if(auto found = m_structureMap.find(origin); found != m_structureMap.end())
		return false;

    {
		ivec2s size = { 0, 0 };

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
            
            offset += m_mapSize.x;
        }
    }

	const auto entity = m_registry.create();
	m_registry.emplace<ivec4s>(entity, bounds);
	
	auto& structure = m_registry.emplace<Structure>(entity);
	structure.type = type;
	structure.armor = structure.maxArmor = get_armor_of(type);
	createGraphicsForEntity(entity);
	
	auto setup_tiles_on_mask = [this, origin, entity](int32_t width, int32_t height, char symbol = 'B') -> void
	{
		int32_t offset = origin;

		for (int32_t i = 0; i < height; ++i)
		{
			for (int32_t j = 0; j < width; ++j)
			{
				m_tileMask[offset + j] = symbol;
				m_structureMap[offset + j] = entity;
			}

			offset += m_mapSize.x;
		}
	};

	switch(type)
	{
		case Structure::Type::SLAB_1x1:          setup_tiles_on_mask(1, 1, 'C'); break;
		case Structure::Type::PALACE:            setup_tiles_on_mask(3, 3);      break;
		case Structure::Type::VEHICLE:           setup_tiles_on_mask(3, 2);      break;
		case Structure::Type::HIGH_TECH:         setup_tiles_on_mask(2, 2);      break;
		case Structure::Type::CONSTRUCTION_YARD: setup_tiles_on_mask(2, 2);      break;
		case Structure::Type::WIND_TRAP:         setup_tiles_on_mask(2, 2);      break;
		case Structure::Type::BARRACKS:          setup_tiles_on_mask(2, 2);      break;
		case Structure::Type::STARPORT:          setup_tiles_on_mask(3, 3);      break;
		case Structure::Type::REFINERY:          setup_tiles_on_mask(3, 2);      break;
		case Structure::Type::REPAIR:            setup_tiles_on_mask(3, 2);      break;
		case Structure::Type::WALL:              setup_tiles_on_mask(1, 1, 'W'); break;
		case Structure::Type::TURRET:            setup_tiles_on_mask(1, 1);      break;
		case Structure::Type::ROCKET_TURRET:     setup_tiles_on_mask(1, 1);      break;
		case Structure::Type::SILO:              setup_tiles_on_mask(2, 2);      break;
		case Structure::Type::OUTPOST:           setup_tiles_on_mask(2, 2);      break;

		default: break;
	}

	if(type == Structure::Type::WALL)
		updateWall(origin, 2);

	return true;
}


uint32_t Builder::getVertexBuffer() const noexcept
{
	return m_vertexBuffer;
}


void Builder::createGraphicsForEntity(const entt::entity entity) noexcept
{
	if(m_mappedStorage)
	{
		const uint32_t id = m_registry.storage<Structure>().size() - 1;

		auto& building = m_registry.get<Structure>(entity);
		building.id = id;
		building.frame = (id << 2);

		const auto bounds = m_registry.get<ivec4s>(entity);
		const vec4s texCoords = get_texcoords_of_structure(building.type, m_textureSize.x, m_textureSize.y);

		const float vertices[] = 
		{
			static_cast<float>(bounds.x), static_cast<float>(bounds.y), texCoords.x, texCoords.y,
			static_cast<float>(bounds.z), static_cast<float>(bounds.y), texCoords.z, texCoords.y,
			static_cast<float>(bounds.z), static_cast<float>(bounds.w), texCoords.z, texCoords.w,
			static_cast<float>(bounds.x), static_cast<float>(bounds.w), texCoords.x, texCoords.w
		};

		float* bytes = static_cast<float*>(m_mappedStorage);
		bytes += id * std::size(vertices);
		memcpy(bytes, vertices, sizeof(vertices));
	}
}


void Builder::updateWall(int32_t origin, int32_t level) noexcept
{
    if(level > 0)
	{
		int32_t left   = origin - 1;
		int32_t top    = origin - m_mapSize.x;
		int32_t right  = origin + 1;
		int32_t bottom = origin + m_mapSize.y;

		const char* field = m_tileMask.data();
		const int32_t tileCount = m_mapSize.x * m_mapSize.y;

		bool a = (left >= 0)          ? (field[left]   == 'W') : false;
		bool b = (top >= 0)           ? (field[top]    == 'W') : false;
		bool c = (right < tileCount)  ? (field[right]  == 'W') : false;
		bool d = (bottom < tileCount) ? (field[bottom] == 'W') : false;

		const auto texCoords = get_texcoords_of_custom_wall(compute_wall_type(a, b, c, d), m_textureSize);
		const auto entity = m_structureMap[origin];

		if(m_mappedStorage)
		{
			const auto& building = m_registry.get<Structure>(entity);

			float* bytes = static_cast<float*>(m_mappedStorage);
			bytes += building.id * 16; // TODO: fix magic num

			bytes[2] = texCoords.x;
			bytes[3] = texCoords.y;

			bytes[6] = texCoords.z;
			bytes[7] = texCoords.y;

			bytes[10] = texCoords.z;
			bytes[11] = texCoords.w;

			bytes[14] = texCoords.x;
			bytes[15] = texCoords.w;

			if (a) updateWall(left, level - 1);
			if (b) updateWall(top, level - 1);
			if (c) updateWall(right, level - 1);
			if (d) updateWall(bottom, level - 1);
		}
	}
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


vec4s get_texcoords_of_custom_wall(const WallCellType type, ivec2s textureSize) noexcept
{
	const vec4s ratio = { 1.f / textureSize.x, 1.f / textureSize.y };
	vec4s tc; // texture coords

    switch (type)
    {
		default:
        case WallCellType::DOT:               { tc =  { 0.f,   0.f, 32.f, 32.f }; break; } 
        case WallCellType::LEFT_RIGHT:        { tc =  { 32.f,  0.f, 32.f, 32.f }; break; } 
        case WallCellType::BOTTOM_TOP:        { tc =  { 64.f,  0.f, 32.f, 32.f }; break; } 
        case WallCellType::TOP_RIGHT:         { tc =  { 96.f,  0.f, 32.f, 32.f }; break; } 
        case WallCellType::RIGHT_BOTTOM:      { tc =  { 128.f, 0.f, 32.f, 32.f }; break; }
        case WallCellType::BOTTOM_LEFT:       { tc =  { 160.f, 0.f, 32.f, 32.f }; break; }
        case WallCellType::LEFT_TOP:          { tc =  { 192.f, 0.f, 32.f, 32.f }; break; }
        case WallCellType::TOP_RIGHT_BOTTOM:  { tc =  { 224.f, 0.f, 32.f, 32.f }; break; } 
        case WallCellType::RIGHT_BOTTOM_LEFT: { tc =  { 256.f, 0.f, 32.f, 32.f }; break; } 
        case WallCellType::BOTTOM_LEFT_TOP:   { tc =  { 288.f, 0.f, 32.f, 32.f }; break; }
        case WallCellType::LEFT_TOP_RIGHT:    { tc =  { 320.f, 0.f, 32.f, 32.f }; break; }
        case WallCellType::CROSS:             { tc =  { 352.f, 0.f, 32.f, 32.f }; break; }
    }

	return { tc.x * ratio.x, tc.y * ratio.y, (tc.x + tc.z) * ratio.x, (tc.y + tc.w) * ratio.y };
}


vec4s get_texcoords_of_structure(const Structure::Type type, float width, float height) noexcept
{
	const vec4s ratio = { 1.f / width, 1.f / height };
	vec4s tc; // texture coords

	switch (type)
	{
		case Structure::Type::SLAB_1x1:          { tc = { 0.f,   160.f, 32.f, 32.f }; break; }
		case Structure::Type::PALACE:            { tc = { 64.f,  96.f , 96.f, 96.f }; break; }
		case Structure::Type::VEHICLE:           { tc = { 256.f, 32.f , 96.f, 64.f }; break; }
		case Structure::Type::HIGH_TECH:         { tc = { 160.f, 96.f , 64.f, 64.f }; break; }
		case Structure::Type::CONSTRUCTION_YARD: { tc = { 0.f,   32.f , 64.f, 64.f }; break; }
		case Structure::Type::WIND_TRAP:         { tc = { 64.f,  32.f , 64.f, 64.f }; break; }
		case Structure::Type::BARRACKS:          { tc = { 0.f,   96.f , 64.f, 64.f }; break; }
		case Structure::Type::STARPORT:          { tc = { 0.f,   192.f, 96.f, 96.f }; break; }
		case Structure::Type::REFINERY:          { tc = { 416.f, 0.f  , 96.f, 64.f }; break; }
		case Structure::Type::REPAIR:            { tc = { 224.f, 96.f , 96.f, 64.f }; break; }
		case Structure::Type::WALL:              { tc = { 0.f,   0.f  , 32.f, 32.f }; break; }
		case Structure::Type::TURRET:            { tc = { 192.f, 288.f, 32.f, 32.f }; break; }
		case Structure::Type::ROCKET_TURRET:     { tc = { 448.f, 288.f, 32.f, 32.f }; break; }
		case Structure::Type::SILO:              { tc = { 192.f, 32.f , 64.f, 64.f }; break; }
		case Structure::Type::OUTPOST:           { tc = { 128.f, 32.f , 64.f, 64.f }; break; }

		default: return { 0.f, 0.f, 0.f, 0.f };
	}

	return { tc.x * ratio.x, tc.y * ratio.y, (tc.x + tc.z) * ratio.x, (tc.y + tc.w) * ratio.y };
}


ivec4s get_bounds_of(const Structure::Type type, const ivec2s cell, const ivec2s tileSize) noexcept
{
	const ivec2s offset = { cell.x * tileSize.x, cell.y * tileSize.y }; 

	switch (type)
	{
		case Structure::Type::SLAB_1x1:          return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case Structure::Type::PALACE:            return { offset.x, offset.y, offset.x + 96, offset.y + 96 };
		case Structure::Type::VEHICLE:           return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case Structure::Type::HIGH_TECH:         return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case Structure::Type::CONSTRUCTION_YARD: return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case Structure::Type::WIND_TRAP:         return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case Structure::Type::BARRACKS:          return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case Structure::Type::STARPORT:          return { offset.x, offset.y, offset.x + 96, offset.y + 96 };
		case Structure::Type::REFINERY:          return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case Structure::Type::REPAIR:            return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case Structure::Type::WALL:              return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case Structure::Type::TURRET:            return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case Structure::Type::ROCKET_TURRET:     return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case Structure::Type::SILO:              return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case Structure::Type::OUTPOST:           return { offset.x, offset.y, offset.x + 64, offset.y + 64 }; 

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