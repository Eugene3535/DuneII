#include <cstring>

#include <glad/glad.h>
#include <cglm/struct/ivec2.h>
#include <cglm/call/aabb2d.h>

#include "resources/files/FileProvider.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/Engine.hpp"
#include "game/scenes/mission/loader/TiledMapLoader.hpp"
#include "game/scenes/mission/tilemap/Tilemap.hpp"


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


static WallCellType compute_wall_type(bool left, bool top, bool right, bool bottom)                         noexcept;
static vec4s        get_texcoords_of_custom_wall(const WallCellType type, ivec2s textureSize)               noexcept;
static vec4s        get_texcoords_of_structure(const StructureInfo::Type type, float width, float height)   noexcept;
static ivec4s       get_bounds_of(const StructureInfo::Type type, const ivec2s cell, const ivec2s tileSize) noexcept; // cell value must be presented in tiles, not pixels
static int32_t      get_armor_of(const StructureInfo::Type type)                                            noexcept;


Tilemap::Tilemap(entt::registry& registry, const Engine* engine) noexcept:
	Transform2D(),
    m_registry(registry),
	m_engine(engine),
	m_vertexBuffer(0),
	m_mappedStorage(nullptr),
	m_textureSize(glms_ivec2_zero()),
	m_mapSize(glms_ivec2_zero()),
	m_tileSize(glms_ivec2_zero())
{

}


Tilemap::~Tilemap()
{
	cleanupGraphicsResources();
}


bool Tilemap::createFromLoader(const TiledMapLoader& loader) noexcept
{
	createGraphicsResources(loader.getVertices(), loader.getIndices());
	m_structureMask.clear();
	
	m_mapSize = loader.getMapSize();
	m_tileSize = loader.getTileSize();
	m_tileMask = loader.getTileMask();
	m_structureMask.resize(m_mapSize.x * m_mapSize.y, entt::null);

	glGetTextureLevelParameteriv(m_buildings.texture, 0, GL_TEXTURE_WIDTH, &m_textureSize.x);
	glGetTextureLevelParameteriv(m_buildings.texture, 0, GL_TEXTURE_HEIGHT, &m_textureSize.y);

	auto get_structure_enum = [](const std::string& name) -> StructureInfo::Type
	{
		if(name == "Wall")          return StructureInfo::Type::WALL;
		if(name == "Refinery")      return StructureInfo::Type::REFINERY;
		if(name == "ConstructYard") return StructureInfo::Type::CONSTRUCTION_YARD ;
		if(name == "WindTrap")      return StructureInfo::Type::WIND_TRAP;
		if(name == "Outpost")       return StructureInfo::Type::OUTPOST;
		if(name == "Silo")          return StructureInfo::Type::SILO;
		if(name == "Vehicle")       return StructureInfo::Type::VEHICLE;
		if(name == "Barracks")      return StructureInfo::Type::BARRACKS;
		if(name == "Palace")        return StructureInfo::Type::PALACE;
		if(name == "HighTech")      return StructureInfo::Type::HIGH_TECH;
		if(name == "Repair")        return StructureInfo::Type::REPAIR;
		if(name == "Slab_1x1")      return StructureInfo::Type::SLAB_1x1;
		if(name == "Starport")      return StructureInfo::Type::STARPORT;
		if(name == "Turret")        return StructureInfo::Type::TURRET;
		if(name == "RocketTurret")  return StructureInfo::Type::ROCKET_TURRET;

		return StructureInfo::Type::INVALID;
	};

	auto objects = loader.getObjects();

	if(objects.empty())
		return false;

	struct Base
	{
		vec2 aabb[2];
		bool exists;
	};

	auto get_aabb_of_base = [&objects](HouseType houseName) -> Base
	{
		Base base = {.exists = false};

		for(const auto& object : objects)
		{
			if(object.name == "Base")
			{
				auto found = std::find_if(object.properties.begin(), object.properties.end(), [houseName](const TiledMapLoader::Object::Property& property)
				{
					return houseName == static_cast<HouseType>(std::stoul(property.value));
				});

				if(found != object.properties.end())
				{
					base.aabb[0][0] = object.coords.x;
					base.aabb[0][1] = object.coords.y;
					base.aabb[1][0] = object.coords.x + object.size.x;
					base.aabb[1][1] = object.coords.y + object.size.y;
					base.exists = true;

					return base;
				}
			}
		}

		return base;
	};

	auto atreidesBase  = get_aabb_of_base(HouseType::Atreides);
	auto ordosBase     = get_aabb_of_base(HouseType::Ordos);
	auto harkonnenBase = get_aabb_of_base(HouseType::Harkonnen);		
	// ... and other houses

	for(const auto& object : objects)
	{
		if (object.type == "Structure")
		{
			HouseType owner = HouseType::INVALID;
			vec2 objectAABB[2];
			objectAABB[0][0] = object.coords.x * m_tileSize.x;
			objectAABB[0][1] = object.coords.y * m_tileSize.y;
			objectAABB[1][0] = (object.coords.x + object.size.x) * m_tileSize.x;
			objectAABB[1][1] = (object.coords.y + object.size.y) * m_tileSize.y;

			if(harkonnenBase.exists && glmc_aabb2d_aabb(harkonnenBase.aabb, objectAABB))
			{
				owner = HouseType::Harkonnen;
			}
			else if(ordosBase.exists && glmc_aabb2d_aabb(ordosBase.aabb, objectAABB))
			{
				owner = HouseType::Ordos;
			}
			else if(atreidesBase.exists && glmc_aabb2d_aabb(atreidesBase.aabb, objectAABB))
			{
				owner = HouseType::Atreides;
			}

			if(owner != HouseType::INVALID)
			{
				const auto type = get_structure_enum(object.name);
				putStructure(owner, type, object.coords);
			}
		}
	}

    return true;
}


bool Tilemap::putStructure(const HouseType owner, const StructureInfo::Type type, const ivec2s cell) noexcept
{
	if(auto size = m_registry.storage<StructureInfo>().size(); size >= STRUCTURE_LIMIT_ON_MAP)
		return false;

	if(type >= StructureInfo::Type::MAX)
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

	if(m_structureMask[static_cast<size_t>(origin)] != entt::null)
		return false;

    {// We check that the building will fit completely on the rocky soil.
		ivec2s size = { 0, 0 };

        switch (type)
        {
            case StructureInfo::Type::SLAB_1x1:          size = { 1, 1 }; break;
			case StructureInfo::Type::PALACE:            size = { 3, 3 }; break;
			case StructureInfo::Type::VEHICLE:           size = { 3, 2 }; break;
			case StructureInfo::Type::HIGH_TECH:         size = { 2, 2 }; break;
            case StructureInfo::Type::CONSTRUCTION_YARD: size = { 2, 2 }; break;
			case StructureInfo::Type::WIND_TRAP:         size = { 2, 2 }; break;
            case StructureInfo::Type::BARRACKS:          size = { 2, 2 }; break;
			case StructureInfo::Type::STARPORT:          size = { 3, 3 }; break;
			case StructureInfo::Type::REFINERY:          size = { 3, 2 }; break;
			case StructureInfo::Type::REPAIR:            size = { 3, 2 }; break;
            case StructureInfo::Type::WALL:              size = { 1, 1 }; break;
            case StructureInfo::Type::TURRET:            size = { 1, 1 }; break;
            case StructureInfo::Type::ROCKET_TURRET:     size = { 1, 1 }; break;
            case StructureInfo::Type::SILO:              size = { 2, 2 }; break;
            case StructureInfo::Type::OUTPOST:           size = { 2, 2 }; break;

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
	
	auto& structure = m_registry.emplace<StructureInfo>(entity);
	structure.owner = owner;
	structure.type = type;
	structure.armor = structure.maxArmor = get_armor_of(type);
	createGraphicsForEntity(entity);

	const bool hasConstructionPreviews = ((type == StructureInfo::Type::VEHICLE)           ||
									      (type == StructureInfo::Type::HIGH_TECH)         || 
									      (type == StructureInfo::Type::CONSTRUCTION_YARD) || 
									      (type == StructureInfo::Type::BARRACKS)          ||
										  (type == StructureInfo::Type::STARPORT));
	
	if(hasConstructionPreviews)
	{
		const GameInfo* info = m_engine->getInfo();
		auto previews = info->getPreviewIconList(owner, type, 8);

		if(!previews.empty())
			m_registry.emplace<std::vector<PreviewType>>(entity, previews);
	}

	auto setup_tiles_on_mask = [this, origin, entity](int32_t width, int32_t height, char symbol = 'B') -> void
	{
		int32_t offset = origin;

		for (int32_t i = 0; i < height; ++i)
		{
			for (int32_t j = 0; j < width; ++j)
			{
				m_tileMask[offset + j] = symbol;
				m_structureMask[offset + j] = entity;
			}

			offset += m_mapSize.x;
		}
	};

	switch(type)
	{
		case StructureInfo::Type::SLAB_1x1:          setup_tiles_on_mask(1, 1, 'C'); break;
		case StructureInfo::Type::PALACE:            setup_tiles_on_mask(3, 3);      break;
		case StructureInfo::Type::VEHICLE:           setup_tiles_on_mask(3, 2);      break;
		case StructureInfo::Type::HIGH_TECH:         setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::CONSTRUCTION_YARD: setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::WIND_TRAP:         setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::BARRACKS:          setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::STARPORT:          setup_tiles_on_mask(3, 3);      break;
		case StructureInfo::Type::REFINERY:          setup_tiles_on_mask(3, 2);      break;
		case StructureInfo::Type::REPAIR:            setup_tiles_on_mask(3, 2);      break;
		case StructureInfo::Type::WALL:              setup_tiles_on_mask(1, 1, 'W'); break;
		case StructureInfo::Type::TURRET:            setup_tiles_on_mask(1, 1);      break;
		case StructureInfo::Type::ROCKET_TURRET:     setup_tiles_on_mask(1, 1);      break;
		case StructureInfo::Type::SILO:              setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::OUTPOST:           setup_tiles_on_mask(2, 2);      break;

		default: break;
	}

	if(type == StructureInfo::Type::WALL)
		updateWall(origin, 2);

	return true;
}


void Tilemap::draw() const noexcept
{
//  Landscape
	glUseProgram(m_landscape.program);
	glBindTextureUnit(0, m_landscape.texture);
	glBindVertexArray(m_landscape.vao);
	glDrawElements(GL_TRIANGLES, m_landscape.count, GL_UNSIGNED_INT, nullptr);
	glBindTextureUnit(0, 0);

//  Structures
	glBindTextureUnit(0, m_buildings.texture);
	glBindVertexArray(m_buildings.vao);

	auto view = m_registry.view<const StructureInfo>();

	view.each([](const StructureInfo& building) 
	{
		glDrawArrays(GL_TRIANGLE_FAN, building.frame, 4);
	});

	glBindTextureUnit(0, 0);
}


uint32_t Tilemap::getVertexBuffer() const noexcept
{
	return m_vertexBuffer;
}


entt::entity Tilemap::getEntityUnderCursor(const vec2s point) const noexcept
{
	const ivec2s tile = { static_cast<int>(point.x) / m_tileSize.x, static_cast<int>(point.y) / m_tileSize.y };
	const int32_t origin = tile.y * m_mapSize.x + tile.x;

	if(m_structureMask[static_cast<size_t>(origin)] != entt::null)
		return m_structureMask[static_cast<size_t>(origin)];
	
	return entt::null;
}


entt::registry& Tilemap::getRegistry() const noexcept
{
	return m_registry;
}


bool Tilemap::createGraphicsResources(std::span<const vec4s> vertices, std::span<const uint32_t> indices) noexcept
{
	if(m_mappedStorage)
		return true;

	memset(&m_landscape, 0, sizeof(mesh::Landscape)); 
    memset(&m_buildings, 0, sizeof(mesh::Buildings));

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

	glGenTextures(1, &m_landscape.texture);
    glGenBuffers(2, m_landscape.vbo);
    glGenVertexArrays(1, &m_landscape.vao);

    Texture landscapeTexture = {.handle = m_landscape.texture };

    if(!landscapeTexture.loadFromFile(FileProvider::findPathToFile(LANDSCAPE_PNG)))
        return false;

    m_landscape.texture = landscapeTexture.handle;

    if(m_landscape.program = m_engine->getShaderProgram("tilemap"); m_landscape.program == 0)
        return false;

	glBindBuffer(GL_ARRAY_BUFFER, m_landscape.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_landscape.vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size_bytes()), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float4 };
	VertexArrayObject::createVertexInputState(m_landscape.vao, m_landscape.vbo[0], attributes);
	
	glBindVertexArray(m_landscape.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_landscape.vbo[1]);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_landscape.vao = m_landscape.vao;
	m_landscape.count = indices.size();

	glGenTextures(1, &m_buildings.texture);
	glGenVertexArrays(1, &m_buildings.vao);

	VertexArrayObject::createVertexInputState(m_buildings.vao, m_vertexBuffer, attributes);

	Texture buildingTexture = {.handle = m_buildings.texture };

	if(!buildingTexture.loadFromFile(FileProvider::findPathToFile(STRUCTURES_PNG)))
		return false;

	return true;
}


void Tilemap::cleanupGraphicsResources() noexcept
{
	GLuint textures[]            = { m_landscape.texture, m_buildings.texture               };
	GLuint vertexArrayObjects[]  = { m_landscape.vao, m_buildings.vao                       };
	GLuint vertexBufferObjects[] = { m_vertexBuffer, m_landscape.vbo[0], m_landscape.vbo[1] };

	glDeleteTextures(std::size(textures), textures);
	glDeleteVertexArrays(std::size(vertexArrayObjects), vertexArrayObjects);

	if(m_mappedStorage)
		glUnmapNamedBuffer(m_vertexBuffer);
		
    glDeleteBuffers(std::size(vertexBufferObjects), vertexBufferObjects);
}


void Tilemap::createGraphicsForEntity(const entt::entity entity) noexcept
{
	if(m_mappedStorage)
	{
		const uint32_t id = m_registry.storage<StructureInfo>().size() - 1;

		auto& building = m_registry.get<StructureInfo>(entity);
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


void Tilemap::updateWall(int32_t origin, int32_t level) noexcept
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
		const auto entity = m_structureMask[static_cast<size_t>(origin)];

		if(m_mappedStorage)
		{
			const auto& building = m_registry.get<StructureInfo>(entity);

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


vec4s get_texcoords_of_structure(const StructureInfo::Type type, float width, float height) noexcept
{
	const vec4s ratio = { 1.f / width, 1.f / height };
	vec4s tc; // texture coords

	switch (type)
	{
		case StructureInfo::Type::SLAB_1x1:          { tc = { 0.f,   160.f, 32.f, 32.f }; break; }
		case StructureInfo::Type::PALACE:            { tc = { 64.f,  96.f , 96.f, 96.f }; break; }
		case StructureInfo::Type::VEHICLE:           { tc = { 256.f, 32.f , 96.f, 64.f }; break; }
		case StructureInfo::Type::HIGH_TECH:         { tc = { 160.f, 96.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::CONSTRUCTION_YARD: { tc = { 0.f,   32.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::WIND_TRAP:         { tc = { 64.f,  32.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::BARRACKS:          { tc = { 0.f,   96.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::STARPORT:          { tc = { 0.f,   192.f, 96.f, 96.f }; break; }
		case StructureInfo::Type::REFINERY:          { tc = { 416.f, 0.f  , 96.f, 64.f }; break; }
		case StructureInfo::Type::REPAIR:            { tc = { 224.f, 96.f , 96.f, 64.f }; break; }
		case StructureInfo::Type::WALL:              { tc = { 0.f,   0.f  , 32.f, 32.f }; break; }
		case StructureInfo::Type::TURRET:            { tc = { 192.f, 288.f, 32.f, 32.f }; break; }
		case StructureInfo::Type::ROCKET_TURRET:     { tc = { 448.f, 288.f, 32.f, 32.f }; break; }
		case StructureInfo::Type::SILO:              { tc = { 192.f, 32.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::OUTPOST:           { tc = { 128.f, 32.f , 64.f, 64.f }; break; }

		default: return { 0.f, 0.f, 0.f, 0.f };
	}

	return { tc.x * ratio.x, tc.y * ratio.y, (tc.x + tc.z) * ratio.x, (tc.y + tc.w) * ratio.y };
}


ivec4s get_bounds_of(const StructureInfo::Type type, const ivec2s cell, const ivec2s tileSize) noexcept
{
	const ivec2s offset = { cell.x * tileSize.x, cell.y * tileSize.y }; 

	switch (type)
	{
		case StructureInfo::Type::SLAB_1x1:          return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case StructureInfo::Type::PALACE:            return { offset.x, offset.y, offset.x + 96, offset.y + 96 };
		case StructureInfo::Type::VEHICLE:           return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case StructureInfo::Type::HIGH_TECH:         return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::CONSTRUCTION_YARD: return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::WIND_TRAP:         return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::BARRACKS:          return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::STARPORT:          return { offset.x, offset.y, offset.x + 96, offset.y + 96 };
		case StructureInfo::Type::REFINERY:          return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case StructureInfo::Type::REPAIR:            return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case StructureInfo::Type::WALL:              return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case StructureInfo::Type::TURRET:            return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case StructureInfo::Type::ROCKET_TURRET:     return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case StructureInfo::Type::SILO:              return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::OUTPOST:           return { offset.x, offset.y, offset.x + 64, offset.y + 64 }; 

		default: return { 0, 0, 0, 0 };
	}
}


int32_t get_armor_of(const StructureInfo::Type type) noexcept
{
	switch (type)
	{
		case StructureInfo::Type::SLAB_1x1:          return 40;
		case StructureInfo::Type::PALACE:            return 2000;
		case StructureInfo::Type::VEHICLE:           return 800;
		case StructureInfo::Type::HIGH_TECH:         return 1000;
		case StructureInfo::Type::CONSTRUCTION_YARD: return 800;
		case StructureInfo::Type::WIND_TRAP:         return 400;
		case StructureInfo::Type::BARRACKS:          return 600;
		case StructureInfo::Type::STARPORT:          return 1000;
		case StructureInfo::Type::REFINERY:          return 900;
		case StructureInfo::Type::REPAIR:            return 1800;
		case StructureInfo::Type::WALL:              return 140;
		case StructureInfo::Type::TURRET:            return 025;
		case StructureInfo::Type::ROCKET_TURRET:     return 500;
		case StructureInfo::Type::SILO:              return 300;
		case StructureInfo::Type::OUTPOST:           return 1000; 

		default: return 1;
	}
}