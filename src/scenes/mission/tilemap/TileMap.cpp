#include <cstring>
#include <algorithm>

#include <glad/glad.h>
#include <cglm/struct/ivec2.h>
#include <cglm/call/aabb2d.h>
#include "cglm/struct/affine-mat.h"
#include <magic_enum/magic_enum.hpp>

#include "files/FileProvider.hpp"
#include "files/TiledMapLoader.hpp"
#include "graphics/vao/VertexBufferLayout.hpp"
#include "graphics/texture/Texture2D.hpp"
#include "application/game/Game.hpp"
#include "scenes/mission/tilemap/Tilemap.hpp"


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


TileMap::TileMap(Game* game, entt::registry& registry) noexcept:
	Transform2D(),
	m_game(game),
    m_registry(registry),
	m_textureSize(glms_ivec2_zero()),
	m_mapSize(glms_ivec2_zero()),
	m_tileSize(glms_ivec2_zero())
{

}


TileMap::~TileMap()
{
	cleanupGraphicsResources();
}


bool TileMap::createFromLoader(const TiledMapLoader& loader) noexcept
{
	createGraphicsResources(loader.getVertices(), loader.getIndices());
	m_structureMask.clear();

	m_mapSize = loader.getMapSize();
	m_tileSize = loader.getTileSize();
	m_tileMask = loader.getTileMask();
	m_structureMask.resize(m_mapSize.x * m_mapSize.y, entt::null);

	glGetTextureLevelParameteriv(m_buildings.texture, 0, GL_TEXTURE_WIDTH, &m_textureSize.x);
	glGetTextureLevelParameteriv(m_buildings.texture, 0, GL_TEXTURE_HEIGHT, &m_textureSize.y);

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
					if (auto value = std::get_if<int>(&property.value))
					{
						return (houseName == static_cast<HouseType>(*value));
					}
					
					return false;
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

	auto get_structure_enum = [](std::string_view name) -> StructureInfo::Type
	{
		auto result = magic_enum::enum_cast<StructureInfo::Type>(name);

		return result.value_or(StructureInfo::Type::Undefined);
	};

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


bool TileMap::putStructure(const HouseType owner, const StructureInfo::Type type, const ivec2s cell) noexcept
{
	if(auto size = m_registry.storage<StructureInfo>().size(); size >= STRUCTURE_LIMIT_ON_MAP)
		return false;

	if(type >= StructureInfo::Type::Max)
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
		auto get_structure_size_in_tiles = [](StructureInfo::Type type) -> ivec2s
		{
			switch (type)
			{
				case StructureInfo::Type::Slab_1x1:         return { 1, 1 };
				case StructureInfo::Type::Palace:           return { 3, 3 };
				case StructureInfo::Type::Vehicle:          return { 3, 2 };
				case StructureInfo::Type::HighTech:         return { 2, 2 };
				case StructureInfo::Type::ConstructionYard: return { 2, 2 };
				case StructureInfo::Type::WindTrap:         return { 2, 2 };
				case StructureInfo::Type::Barracks:         return { 2, 2 };
				case StructureInfo::Type::Starport:         return { 3, 3 };
				case StructureInfo::Type::Refinery:         return { 3, 2 };
				case StructureInfo::Type::Repair:           return { 3, 2 };
				case StructureInfo::Type::Wall:             return { 1, 1 };
				case StructureInfo::Type::Turret:           return { 1, 1 };
				case StructureInfo::Type::RocketTurret:     return { 1, 1 };
				case StructureInfo::Type::Silo:             return { 2, 2 };
				case StructureInfo::Type::Outpost:          return { 2, 2 };

				default: return { 0, 0 };
			}
		};

		const ivec2s size = get_structure_size_in_tiles(type);
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

	const bool hasConstructionPreviews = ((type == StructureInfo::Type::Vehicle)          ||
									      (type == StructureInfo::Type::HighTech)         ||
									      (type == StructureInfo::Type::ConstructionYard) ||
									      (type == StructureInfo::Type::Barracks)         ||
										  (type == StructureInfo::Type::Starport));

	if(hasConstructionPreviews)
	{
		const GameInfo* info = m_game->getInfo();
		auto previews = info->getPreviewIconList(owner, type, 8);

		if (!previews.empty())
			m_registry.emplace<std::vector<EntityIcon>>(entity, previews);

		// m_registry.emplace<Component::Construction>(entity); TODO: optinization
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
		case StructureInfo::Type::Slab_1x1:         setup_tiles_on_mask(1, 1, 'C'); break;
		case StructureInfo::Type::Palace:           setup_tiles_on_mask(3, 3);      break;
		case StructureInfo::Type::Vehicle:          setup_tiles_on_mask(3, 2);      break;
		case StructureInfo::Type::HighTech:         setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::ConstructionYard: setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::WindTrap:         setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::Barracks:         setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::Starport:         setup_tiles_on_mask(3, 3);      break;
		case StructureInfo::Type::Refinery:         setup_tiles_on_mask(3, 2);      break;
		case StructureInfo::Type::Repair:           setup_tiles_on_mask(3, 2);      break;
		case StructureInfo::Type::Wall:             setup_tiles_on_mask(1, 1, 'W'); break;
		case StructureInfo::Type::Turret:           setup_tiles_on_mask(1, 1);      break;
		case StructureInfo::Type::RocketTurret:     setup_tiles_on_mask(1, 1);      break;
		case StructureInfo::Type::Silo:             setup_tiles_on_mask(2, 2);      break;
		case StructureInfo::Type::Outpost:          setup_tiles_on_mask(2, 2);      break;

		default: break;
	}

	if(type == StructureInfo::Type::Wall)
		updateWall(origin, 2);

	return true;
}


void TileMap::draw(const mat4s& projection) const noexcept
{
    mat4s uniformMatrix = projection;
    mat4s modelView     = getMatrix();
    mat4s result        = glms_mul(uniformMatrix, modelView);
    m_game->updateUniformBuffer(result);

//  Landscape
	glUseProgram(m_landscape.program);
	glBindTextureUnit(0, m_landscape.texture);
	glBindVertexArray(m_landscape.vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_landscape.count, GL_UNSIGNED_INT, nullptr);
	glBindTextureUnit(0, 0);

//  Structures
	glBindTextureUnit(0, m_buildings.texture);
	glBindVertexArray(m_buildings.vertexArrayObject);

	auto view = m_registry.view<const StructureInfo>();

	view.each([](const StructureInfo& building)
	{
		glDrawArrays(GL_TRIANGLE_FAN, building.frame, 4);
	});

	glBindTextureUnit(0, 0);
}


entt::entity TileMap::getEntityUnderCursor(const vec2s point) const noexcept
{
	const ivec2s tile = { static_cast<int>(point.x) / m_tileSize.x, static_cast<int>(point.y) / m_tileSize.y };
	const int32_t origin = tile.y * m_mapSize.x + tile.x;

	if(m_structureMask[static_cast<size_t>(origin)] != entt::null)
		return m_structureMask[static_cast<size_t>(origin)];

	return entt::null;
}


entt::registry& TileMap::getRegistry() const noexcept
{
	return m_registry;
}


bool TileMap::createGraphicsResources(std::span<const vec4s> vertices, std::span<const uint32_t> indices) noexcept
{
	memset(&m_landscape, 0, sizeof(m_landscape));
    memset(&m_buildings, 0, sizeof(m_buildings));

//  Landscape
	glCreateBuffers(1, &m_landscape.vertexBufferObject);
    glNamedBufferData(m_landscape.vertexBufferObject, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(), GL_DYNAMIC_DRAW);
	glGenBuffers(1, &m_landscape.indexBufferObject);
	glCreateTextures(GL_TEXTURE_2D, 1, &m_landscape.texture);
    glGenVertexArrays(1, &m_landscape.vertexArrayObject);

    Texture2D landscapeTexture(m_landscape.texture);

    if (!landscapeTexture.loadFromFile(FileProvider::findPathToFile(LANDSCAPE_PNG)))
        return false;

    if (m_landscape.program = m_game->getShaderProgram("tilemap"); m_landscape.program == 0)
        return false;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_landscape.indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size_bytes()), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float4 };
	VertexBufferLayout layout(attributes);
	layout.createVertexInputState(m_landscape.vertexArrayObject, m_landscape.vertexBufferObject);

	glBindVertexArray(m_landscape.vertexArrayObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_landscape.indexBufferObject);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_landscape.count = indices.size();

//  Buildings
	glCreateBuffers(1, &m_buildings.vertexBufferObject);

	glNamedBufferStorage(
		m_buildings.vertexBufferObject,
		STRUCTURE_LIMIT_ON_MAP * sizeof(vec4s) * 4,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT |
		GL_MAP_WRITE_BIT |
		GL_MAP_PERSISTENT_BIT |
		GL_MAP_COHERENT_BIT
	);

	m_buildings.mappedStorage = glMapNamedBufferRange(
		m_buildings.vertexBufferObject,
		0,
		STRUCTURE_LIMIT_ON_MAP * sizeof(vec4s) * 4,
		GL_MAP_WRITE_BIT |
		GL_MAP_PERSISTENT_BIT |
		GL_MAP_COHERENT_BIT |
		GL_MAP_UNSYNCHRONIZED_BIT
	);

	glGenVertexArrays(1, &m_buildings.vertexArrayObject);
	layout.createVertexInputState(m_buildings.vertexArrayObject, m_buildings.vertexBufferObject);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_buildings.texture);
	Texture2D buildingTexture(m_buildings.texture);

	if(!buildingTexture.loadFromFile(FileProvider::findPathToFile(STRUCTURES_PNG)))
		return false;

	return true;
}


void TileMap::cleanupGraphicsResources() noexcept
{
	GLint mapped;
	glGetNamedBufferParameteriv(m_buildings.vertexBufferObject, GL_BUFFER_MAPPED, &mapped);

	if (mapped == GL_TRUE)
		glUnmapNamedBuffer(m_buildings.vertexBufferObject);

	GLuint textures[]            = { m_landscape.texture, m_buildings.texture                                                      };
	GLuint vertexArrayObjects[]  = { m_landscape.vertexArrayObject, m_buildings.vertexArrayObject                                  };
	GLuint vertexBufferObjects[] = { m_landscape.vertexBufferObject, m_landscape.indexBufferObject, m_buildings.vertexBufferObject };

	glDeleteVertexArrays(std::size(vertexArrayObjects), vertexArrayObjects);
	glDeleteBuffers(std::size(vertexBufferObjects), vertexBufferObjects);
	glDeleteTextures(std::size(textures), textures);
}


void TileMap::createGraphicsForEntity(const entt::entity entity) noexcept
{
	if(m_buildings.mappedStorage)
	{
		const uint32_t index = m_registry.storage<StructureInfo>().size() - 1;

		auto& building = m_registry.get<StructureInfo>(entity);
		building.stride = index;
		building.frame = (index << 2);

		const auto bounds = m_registry.get<ivec4s>(entity);
		const vec4s texCoords = get_texcoords_of_structure(building.type, m_textureSize.x, m_textureSize.y);

		const float vertices[] =
		{
			static_cast<float>(bounds.x), static_cast<float>(bounds.y), texCoords.x, texCoords.y,
			static_cast<float>(bounds.z), static_cast<float>(bounds.y), texCoords.z, texCoords.y,
			static_cast<float>(bounds.z), static_cast<float>(bounds.w), texCoords.z, texCoords.w,
			static_cast<float>(bounds.x), static_cast<float>(bounds.w), texCoords.x, texCoords.w
		};

		float* bytes = static_cast<float*>(m_buildings.mappedStorage);
		bytes += index * std::size(vertices);
		memcpy(bytes, vertices, sizeof(vertices));
	}
}


void TileMap::updateWall(int32_t origin, int32_t level) noexcept
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

		if(m_buildings.mappedStorage)
		{
			const auto& building = m_registry.get<StructureInfo>(entity);

			float* bytes = static_cast<float*>(m_buildings.mappedStorage);
			bytes += building.stride * 16; // TODO: fix magic num

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
		case StructureInfo::Type::Slab_1x1:         { tc = { 0.f,   160.f, 32.f, 32.f }; break; }
		case StructureInfo::Type::Palace:           { tc = { 64.f,  96.f , 96.f, 96.f }; break; }
		case StructureInfo::Type::Vehicle:          { tc = { 256.f, 32.f , 96.f, 64.f }; break; }
		case StructureInfo::Type::HighTech:         { tc = { 160.f, 96.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::ConstructionYard: { tc = { 0.f,   32.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::WindTrap:         { tc = { 64.f,  32.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::Barracks:         { tc = { 0.f,   96.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::Starport:         { tc = { 0.f,   192.f, 96.f, 96.f }; break; }
		case StructureInfo::Type::Refinery:         { tc = { 416.f, 0.f  , 96.f, 64.f }; break; }
		case StructureInfo::Type::Repair:           { tc = { 224.f, 96.f , 96.f, 64.f }; break; }
		case StructureInfo::Type::Wall:             { tc = { 0.f,   0.f  , 32.f, 32.f }; break; }
		case StructureInfo::Type::Turret:           { tc = { 192.f, 288.f, 32.f, 32.f }; break; }
		case StructureInfo::Type::RocketTurret:     { tc = { 448.f, 288.f, 32.f, 32.f }; break; }
		case StructureInfo::Type::Silo:             { tc = { 192.f, 32.f , 64.f, 64.f }; break; }
		case StructureInfo::Type::Outpost:          { tc = { 128.f, 32.f , 64.f, 64.f }; break; }

		default: return { 0.f, 0.f, 0.f, 0.f };
	}

	return { tc.x * ratio.x, tc.y * ratio.y, (tc.x + tc.z) * ratio.x, (tc.y + tc.w) * ratio.y };
}


ivec4s get_bounds_of(const StructureInfo::Type type, const ivec2s cell, const ivec2s tileSize) noexcept
{
	const ivec2s offset = { cell.x * tileSize.x, cell.y * tileSize.y };

	switch (type)
	{
		case StructureInfo::Type::Slab_1x1:         return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case StructureInfo::Type::Palace:           return { offset.x, offset.y, offset.x + 96, offset.y + 96 };
		case StructureInfo::Type::Vehicle:          return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case StructureInfo::Type::HighTech:         return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::ConstructionYard: return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::WindTrap:         return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::Barracks:         return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::Starport:         return { offset.x, offset.y, offset.x + 96, offset.y + 96 };
		case StructureInfo::Type::Refinery:         return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case StructureInfo::Type::Repair:           return { offset.x, offset.y, offset.x + 96, offset.y + 64 };
		case StructureInfo::Type::Wall:             return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case StructureInfo::Type::Turret:           return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case StructureInfo::Type::RocketTurret:     return { offset.x, offset.y, offset.x + 32, offset.y + 32 };
		case StructureInfo::Type::Silo:             return { offset.x, offset.y, offset.x + 64, offset.y + 64 };
		case StructureInfo::Type::Outpost:          return { offset.x, offset.y, offset.x + 64, offset.y + 64 };

		default: return { 0, 0, 0, 0 };
	}
}


int32_t get_armor_of(const StructureInfo::Type type) noexcept
{
	switch (type)
	{
		case StructureInfo::Type::Slab_1x1:          return 40;
		case StructureInfo::Type::Palace:            return 2000;
		case StructureInfo::Type::Vehicle:           return 800;
		case StructureInfo::Type::HighTech:         return 1000;
		case StructureInfo::Type::ConstructionYard: return 800;
		case StructureInfo::Type::WindTrap:         return 400;
		case StructureInfo::Type::Barracks:          return 600;
		case StructureInfo::Type::Starport:          return 1000;
		case StructureInfo::Type::Refinery:          return 900;
		case StructureInfo::Type::Repair:            return 1800;
		case StructureInfo::Type::Wall:              return 140;
		case StructureInfo::Type::Turret:            return 025;
		case StructureInfo::Type::RocketTurret:     return 500;
		case StructureInfo::Type::Silo:              return 300;
		case StructureInfo::Type::Outpost:           return 1000;

		default: return 1;
	}
}
