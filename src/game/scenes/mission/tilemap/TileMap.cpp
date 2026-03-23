#include "resources/files/FileProvider.hpp"
#include "graphics/quad/Quad.hpp"
#include "game/DuneII.hpp"
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


static WallCellType compute_wall_type(bool left, bool top, bool right, bool bottom)                                     noexcept;
static sf::IntRect  get_texcoords_of_custom_wall(const WallCellType type)                                               noexcept;
static sf::IntRect  get_texcoords_of_structure(const StructureInfo::Type type)                                          noexcept;
static sf::IntRect  get_bounds_of(const StructureInfo::Type type, const sf::Vector2i cell, const sf::Vector2i tileSize) noexcept; // cell value must be presented in tiles, not pixels
static int32_t      get_armor_of(const StructureInfo::Type type)                                                        noexcept;


Tilemap::Tilemap(DuneII* game, entt::registry& registry) noexcept:
	m_game(game),
    m_registry(registry),
	m_landscapeTexture(nullptr),
	m_structureTexture(nullptr)
{

}


Tilemap::~Tilemap() = default;


bool Tilemap::createFromLoader(const TiledMapLoader& loader) noexcept
{
	auto vertices = loader.getVertices();

	m_vertexBuffer.setPrimitiveType(sf::PrimitiveType::Triangles);
	m_vertexBuffer.setUsage(sf::VertexBuffer::Usage::Dynamic);

	if (!m_vertexBuffer.create(vertices.size()))
		return false;

	if (!m_vertexBuffer.update(vertices.data(), vertices.size(), 0))
		return false;

	auto& assets = m_game->assets;

	if (m_landscapeTexture = assets.get<sf::Texture>(LANDSCAPE_PNG); m_landscapeTexture == nullptr)
		return false;

	if (m_structureTexture = assets.get<sf::Texture>(STRUCTURES_PNG); m_structureTexture == nullptr)
		return false;

	m_structureMask.clear();
	m_mapSize = loader.getMapSize();
	m_tileSize = loader.getTileSize();
	m_tileMask = loader.getTileMask();
	m_structureMask.resize(m_mapSize.x * m_mapSize.y, entt::null);

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

	auto get_aabb_of_base = [&objects](HouseType houseName) -> sf::IntRect
	{
		for(const auto& object : objects)
		{
			if(object.name == "Base")
			{
				auto found = std::find_if(object.properties.begin(), object.properties.end(), [houseName](const TiledMapLoader::Object::Property& property)
				{
					return houseName == static_cast<HouseType>(std::stoul(property.value));
				});

				if(found != object.properties.end())
					return sf::IntRect(object.coords, object.size);
			}
		}

		return sf::IntRect();
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
			const sf::IntRect objectAABB(object.coords, object.size);

			if(harkonnenBase != sf::IntRect() && harkonnenBase.findIntersection(objectAABB))
			{
				owner = HouseType::Harkonnen;
			}
			else if(ordosBase != sf::IntRect() && ordosBase.findIntersection(objectAABB))
			{
				owner = HouseType::Ordos;
			}
			else if(atreidesBase != sf::IntRect() && atreidesBase.findIntersection(objectAABB))
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


bool Tilemap::putStructure(const HouseType owner, const StructureInfo::Type type, const sf::Vector2i point) noexcept
{
	if(auto size = m_registry.storage<StructureInfo>().size(); size >= STRUCTURE_LIMIT_ON_MAP)
		return false;

	if(type >= StructureInfo::Type::MAX)
        return false;

    const auto bounds = get_bounds_of(type, point, m_tileSize);

    {// out of bounds (in pixels) ?
        const int32_t mapWidth  = m_mapSize.x * m_tileSize.x;
        const int32_t mapHeight = m_mapSize.y * m_tileSize.y;

        if(bounds.position.x < 0)     return false;
        if(bounds.position.y < 0)     return false;
        if(bounds.size.x > mapWidth)  return false;
        if(bounds.size.y > mapHeight) return false;
    }

	const sf::Vector2i cell = { point.x / m_tileSize.x, point.y / m_tileSize.y };
    const int32_t origin = cell.y * m_mapSize.x + cell.x;

	if(m_structureMask[static_cast<size_t>(origin)] != entt::null)
		return false;

    {// We check that the building will fit completely on the rocky soil.
		sf::Vector2i size = { 0, 0 };

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
	m_registry.emplace<sf::IntRect>(entity, bounds);
	
	auto& structure = m_registry.emplace<StructureInfo>(entity);
	structure.owner = owner;
	structure.type = type;
	structure.armor = structure.maxArmor = get_armor_of(type);

	const sf::IntRect textureRect = get_texcoords_of_structure(type);
	auto& quad = m_registry.emplace<Quad>(entity, m_structureTexture, textureRect);
	quad.setPosition(sf::Vector2f(point));

	const bool hasConstructionPreviews = ((type == StructureInfo::Type::VEHICLE)           ||
									      (type == StructureInfo::Type::HIGH_TECH)         || 
									      (type == StructureInfo::Type::CONSTRUCTION_YARD) || 
									      (type == StructureInfo::Type::BARRACKS)          ||
										  (type == StructureInfo::Type::STARPORT));
	
	// if(hasConstructionPreviews)
	// {
	// 	const GameInfo& info = m_game->getInfo();
	// 	auto previews = info->getPreviewIconList(owner, type, 8);

	// 	if(!previews.empty())
	// 		m_registry.emplace<std::vector<PreviewType>>(entity, previews);
	// }

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


entt::entity Tilemap::getEntityUnderCursor(const sf::Vector2f point) const noexcept
{
	const sf::Vector2i tile = { static_cast<int>(point.x) / m_tileSize.x, static_cast<int>(point.y) / m_tileSize.y };
	const int32_t origin = tile.y * m_mapSize.x + tile.x;

	if(m_structureMask[static_cast<size_t>(origin)] != entt::null)
		return m_structureMask[static_cast<size_t>(origin)];
	
	return entt::null;
}


entt::registry& Tilemap::getRegistry() const noexcept
{
	return m_registry;
}


void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = m_landscapeTexture;
	target.draw(m_vertexBuffer, states);

	auto view = m_registry.view<StructureInfo, Quad>();

	view.each([&target, &states](StructureInfo& building, Quad& quad) 
	{
		target.draw(quad, states);
	});
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

		const auto textureRect = get_texcoords_of_custom_wall(compute_wall_type(a, b, c, d));
		const auto entity = m_structureMask[static_cast<size_t>(origin)];

		if (entity != entt::null)
		{
			auto view = m_registry.view<Quad>();
			auto& quad = view.get<Quad>(entity);
			quad.setTextureRect(textureRect);
		}
		
		if (a) updateWall(left, level - 1);
		if (b) updateWall(top, level - 1);
		if (c) updateWall(right, level - 1);
		if (d) updateWall(bottom, level - 1);
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


sf::IntRect get_texcoords_of_custom_wall(const WallCellType type) noexcept
{
    switch (type)
    {
        case WallCellType::DOT:               return { { 0,   0 }, { 32, 32 } }; 
        case WallCellType::LEFT_RIGHT:        return { { 32,  0 }, { 32, 32 } }; 
        case WallCellType::BOTTOM_TOP:        return { { 64,  0 }, { 32, 32 } }; 
        case WallCellType::TOP_RIGHT:         return { { 96,  0 }, { 32, 32 } }; 
        case WallCellType::RIGHT_BOTTOM:      return { { 128, 0 }, { 32, 32 } };
        case WallCellType::BOTTOM_LEFT:       return { { 160, 0 }, { 32, 32 } };
        case WallCellType::LEFT_TOP:          return { { 192, 0 }, { 32, 32 } };
        case WallCellType::TOP_RIGHT_BOTTOM:  return { { 224, 0 }, { 32, 32 } }; 
        case WallCellType::RIGHT_BOTTOM_LEFT: return { { 256, 0 }, { 32, 32 } }; 
        case WallCellType::BOTTOM_LEFT_TOP:   return { { 288, 0 }, { 32, 32 } };
        case WallCellType::LEFT_TOP_RIGHT:    return { { 320, 0 }, { 32, 32 } };
        case WallCellType::CROSS:             return { { 352, 0 }, { 32, 32 } };

		default: return { { 0, 0 }, { 32, 32 } };
    }
}


sf::IntRect get_texcoords_of_structure(const StructureInfo::Type type) noexcept
{
	switch (type)
	{
		case StructureInfo::Type::SLAB_1x1:          return { { 0,   160 }, { 32, 32 } };
		case StructureInfo::Type::PALACE:            return { { 64,  96  }, { 96, 96 } };
		case StructureInfo::Type::VEHICLE:           return { { 256, 32  }, { 96, 64 } };
		case StructureInfo::Type::HIGH_TECH:         return { { 160, 96  }, { 64, 64 } };
		case StructureInfo::Type::CONSTRUCTION_YARD: return { { 0,   32  }, { 64, 64 } };
		case StructureInfo::Type::WIND_TRAP:         return { { 64,  32  }, { 64, 64 } };
		case StructureInfo::Type::BARRACKS:          return { { 0,   96  }, { 64, 64 } };
		case StructureInfo::Type::STARPORT:          return { { 0,   192 }, { 96, 96 } };
		case StructureInfo::Type::REFINERY:          return { { 416, 0   }, { 96, 64 } };
		case StructureInfo::Type::REPAIR:            return { { 224, 96  }, { 96, 64 } };
		case StructureInfo::Type::WALL:              return { { 0,   0   }, { 32, 32 } };
		case StructureInfo::Type::TURRET:            return { { 192, 288 }, { 32, 32 } };
		case StructureInfo::Type::ROCKET_TURRET:     return { { 448, 288 }, { 32, 32 } };
		case StructureInfo::Type::SILO:              return { { 192, 32  }, { 64, 64 } };
		case StructureInfo::Type::OUTPOST:           return { { 128, 32  }, { 64, 64 } };

		default: return { { 0, 0 }, { 32, 32 } };
	}
}


sf::IntRect get_bounds_of(const StructureInfo::Type type, const sf::Vector2i cell, const sf::Vector2i tileSize) noexcept
{
	switch (type)
	{
		case StructureInfo::Type::SLAB_1x1:          return { { cell.x, cell.y }, { cell.x + 32, cell.y + 32 } };
		case StructureInfo::Type::PALACE:            return { { cell.x, cell.y }, { cell.x + 96, cell.y + 96 } };
		case StructureInfo::Type::VEHICLE:           return { { cell.x, cell.y }, { cell.x + 96, cell.y + 64 } };
		case StructureInfo::Type::HIGH_TECH:         return { { cell.x, cell.y }, { cell.x + 64, cell.y + 64 } };
		case StructureInfo::Type::CONSTRUCTION_YARD: return { { cell.x, cell.y }, { cell.x + 64, cell.y + 64 } };
		case StructureInfo::Type::WIND_TRAP:         return { { cell.x, cell.y }, { cell.x + 64, cell.y + 64 } };
		case StructureInfo::Type::BARRACKS:          return { { cell.x, cell.y }, { cell.x + 64, cell.y + 64 } };
		case StructureInfo::Type::STARPORT:          return { { cell.x, cell.y }, { cell.x + 96, cell.y + 96 } };
		case StructureInfo::Type::REFINERY:          return { { cell.x, cell.y }, { cell.x + 96, cell.y + 64 } };
		case StructureInfo::Type::REPAIR:            return { { cell.x, cell.y }, { cell.x + 96, cell.y + 64 } };
		case StructureInfo::Type::WALL:              return { { cell.x, cell.y }, { cell.x + 32, cell.y + 32 } };
		case StructureInfo::Type::TURRET:            return { { cell.x, cell.y }, { cell.x + 32, cell.y + 32 } };
		case StructureInfo::Type::ROCKET_TURRET:     return { { cell.x, cell.y }, { cell.x + 32, cell.y + 32 } };
		case StructureInfo::Type::SILO:              return { { cell.x, cell.y }, { cell.x + 64, cell.y + 64 } };
		case StructureInfo::Type::OUTPOST:           return { { cell.x, cell.y }, { cell.x + 64, cell.y + 64 } }; 

		default: return { { 0, 0 }, { 32, 32 } };
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