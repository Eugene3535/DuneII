#include <sstream>
#include <algorithm>
#include <cstring>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "common/Defines.hpp"
#include "assets/AssetManager.hpp"
#include "ecs/components/Animation.hpp"
#include "ecs/components/Structure.hpp"
#include "animation/AnimationManager.hpp"
#include "scenes/mission/tilemap/Tile.hpp"
#include "scenes/mission/tilemap/TileMap.hpp"


TileMap::TileMap(entt::registry& registry, AnimationManager& animationManager, AssetManager& assets) noexcept:
	m_registry(registry),
	m_animationManager(animationManager),
	m_assets(assets),
	m_texture(nullptr)
{

}


bool TileMap::loadFromFile(const std::filesystem::path& file_path) noexcept
{
//  Make sure it hasn't been downloaded before
	unload();

	if (file_path.empty())
		return false;

	if(file_path.extension().string() != ".tmx")
		return false;

	m_title = file_path.stem().string();

	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(file_path.string().c_str());
	document->parse<0>(xmlFile.data());

	const auto map_node = document->first_node("map");

	if (!map_node)
		return false;

	if(loadLayers(static_cast<const void*>(map_node)))
		if(loadObjects(static_cast<const void*>(map_node)))
		{
			auto get_area_of = [this](HouseType houseName) -> sf::IntRect
			{
				for(auto& object : m_objects)
				{
					if(object.name == "Area")
					{
						auto found = std::find_if(object.properties.begin(), object.properties.end(), [houseName](const TileMap::Object::Property& property)
						{
							return houseName == static_cast<HouseType>(std::stoul(property.value));
						});

						if(found != object.properties.end())
							return object.bounds;
					}
				}

				return sf::IntRect();
			};

			auto plant_a_flag = [this](const entt::entity entity, const sf::Texture* texture, const Animation* flag, const sf::IntRect& bounds) -> void
			{
				m_registry.emplace<Animation>(entity, *flag);
				auto& sprite = m_registry.emplace<sf::Sprite>(entity, *texture, flag->frames[0]);
				int32_t flagX = bounds.position.x;
				int32_t flagY = bounds.position.y + bounds.size.y - sprite.getTextureRect().size.y;
				sprite.setPosition(sf::Vector2f(flagX, flagY));
			};

			const sf::Texture* flag_texture = m_assets.get<sf::Texture>(FLAGS_PNG);

			if(!flag_texture)
				return false;

			auto harkonnen_area = get_area_of(HouseType::HARKONNEN);		
			auto ordos_area     = get_area_of(HouseType::ORDOS);
			auto atreides_area  = get_area_of(HouseType::ATREIDES);

			auto harkonnen_flag = m_animationManager.getAnimation("HarkonnenFlag");
			auto ordos_flag     = m_animationManager.getAnimation("OrdosFlag");
			auto atreides_flag  = m_animationManager.getAnimation("AtreidesFlag");

			if( ! harkonnen_flag ) return false;
			if( ! ordos_flag     ) return false;
			if( ! atreides_flag  ) return false;

			auto view = m_registry.view<Structure, sf::IntRect>();

			for (auto [entity, structure, bounds] : view.each())
			{
				bool need_to_plant_a_flag = ((structure.type != StructureType::SLAB_1x1)      &&
								             (structure.type != StructureType::SLAB_2x2)      && 
								             (structure.type != StructureType::WALL)          && 
								             (structure.type != StructureType::TURRET)        && 
								             (structure.type != StructureType::ROCKET_TURRET) &&
											  structure.type < StructureType::MAX);

				if(harkonnen_area != sf::IntRect() && bounds.findIntersection(harkonnen_area))
				{
					structure.owner = HouseType::HARKONNEN;

					if(need_to_plant_a_flag)		
						plant_a_flag(entity, flag_texture, harkonnen_flag, bounds);		
				}
				else if(ordos_area != sf::IntRect() && bounds.findIntersection(ordos_area))
				{
					structure.owner = HouseType::ORDOS;

					if(need_to_plant_a_flag)
						plant_a_flag(entity, flag_texture, ordos_flag, bounds);
				}
				else if(atreides_area != sf::IntRect() && bounds.findIntersection(atreides_area))
				{
					structure.owner = HouseType::ATREIDES;

					if(need_to_plant_a_flag)		
						plant_a_flag(entity, flag_texture, atreides_flag, bounds);
				}
			}

			return true;
		}

	return false;
}


void TileMap::unload() noexcept
{
	m_structuresById.clear();
	m_vertices.~VertexBuffer();
	m_texture = nullptr;

	m_objects.clear();
	m_tileMask.clear();
	m_title.clear();

	m_mapSizeInTiles  = { 0, 0 };
	m_mapSizeInPixels = { 0, 0 };
	m_tileSize        = { 0, 0 };
}


bool TileMap::putStructureOnMap(StructureType type, int32_t cellX, int32_t cellY) noexcept
{
	if(type >= StructureType::MAX)
		return false;

	int32_t coordX = cellX * m_tileSize.x;
	int32_t coordY = cellY * m_tileSize.y;
    auto bounds = getBoundsOf(type, coordX, coordY);

    {// out of bounds ?
        int32_t map_width  = m_mapSizeInTiles.x * m_tileSize.x;
        int32_t map_height = m_mapSizeInTiles.y * m_tileSize.y;

        if(bounds.position.x < 0)                          return false;
        if(bounds.position.y < 0)                          return false;
        if(bounds.position.x + bounds.size.x > map_width)  return false;
        if(bounds.position.y + bounds.size.y > map_height) return false;
    }

    const int32_t origin = cellY * m_mapSizeInTiles.x + cellX;

	if(auto found = m_structuresById.find(origin); found != m_structuresById.end())
		return false;

    {// can be constructed ?
        sf::Vector2i size;

        switch (type)
        {
            case StructureType::SLAB_1x1:          size = { 1, 1 }; break;
			case StructureType::PALACE:            size = { 3, 3 }; break;
			case StructureType::VEHICLE:           size = { 3, 2 }; break;
			case StructureType::HIGH_TECH:         size = { 2, 2 }; break;
            case StructureType::CONSTRUCTION_YARD: size = { 2, 2 }; break;
			case StructureType::WIND_TRAP:         size = { 2, 2 }; break;
            case StructureType::BARRACKS:          size = { 2, 2 }; break;
			case StructureType::STARPORT:          size = { 3, 3 }; break;
			case StructureType::REFINERY:          size = { 3, 2 }; break;
			case StructureType::REPAIR:            size = { 3, 2 }; break;
            case StructureType::WALL:              size = { 1, 1 }; break;
            case StructureType::TURRET:            size = { 1, 1 }; break;
            case StructureType::ROCKET_TURRET:     size = { 1, 1 }; break;
            case StructureType::SILO:              size = { 2, 2 }; break;
            case StructureType::OUTPOST:           size = { 2, 2 }; break;

            default: break;
        }

		if(size == sf::Vector2i(0, 0))
			return false;

        const char* mask = m_tileMask.data();
        int32_t offset = origin;

        for (int32_t i = 0; i < size.y; ++i)
        {
            for (int32_t j = 0; j < size.x; ++j)  
                if(mask[offset + j] != 'R')
                    return false;
            
            offset += m_mapSizeInTiles.x;
        }
    }

	if(const auto texture = m_assets.get<sf::Texture>(STRUCTURES_PNG))
	{
		const auto entity = m_registry.create();
		m_registry.emplace<sf::IntRect>(entity, bounds);
		auto& tile = m_registry.emplace<Tile>(entity, texture, getTexCoordsOf(type));
		auto& structure = m_registry.emplace<Structure>(entity);

		structure.type = type;
		tile.setPosition(sf::Vector2f(coordX, coordY));
		structure.hitPoints = structure.maxHitPoints = getHitPointsOf(type);

		auto setup_tiles_on_mask = [this, origin, entity](int32_t width, int32_t height, char symbol = 'B') -> void
		{
			char* mask = m_tileMask.data();
			int32_t offset = origin;

			for (int32_t i = 0; i < height; ++i)
			{
				for (int32_t j = 0; j < width; ++j)			
				{
					mask[offset + j] = symbol;  
					m_structuresById[offset + j] = entity;
				}

				offset += m_mapSizeInTiles.x;
			}
		};

		switch(type)
        {
            case StructureType::SLAB_1x1:          setup_tiles_on_mask(1, 1, 'C'); break;
			case StructureType::PALACE:            setup_tiles_on_mask(3, 3);      break;
			case StructureType::VEHICLE:           setup_tiles_on_mask(3, 2);      break;
			case StructureType::HIGH_TECH:         setup_tiles_on_mask(2, 2);      break;
            case StructureType::CONSTRUCTION_YARD: setup_tiles_on_mask(2, 2);      break;
			case StructureType::WIND_TRAP:         setup_tiles_on_mask(2, 2);      break;
            case StructureType::BARRACKS:          setup_tiles_on_mask(2, 2);      break;
			case StructureType::STARPORT:          setup_tiles_on_mask(3, 3);      break;
			case StructureType::REFINERY:          setup_tiles_on_mask(3, 2);      break;
			case StructureType::REPAIR:            setup_tiles_on_mask(3, 2);      break;
            case StructureType::WALL:              setup_tiles_on_mask(1, 1, 'W'); break;
            case StructureType::TURRET:            setup_tiles_on_mask(1, 1);      break;
            case StructureType::ROCKET_TURRET:     setup_tiles_on_mask(1, 1);      break;
            case StructureType::SILO:              setup_tiles_on_mask(2, 2);      break;
            case StructureType::OUTPOST:           setup_tiles_on_mask(2, 2);      break;

            default: break;
        }

		if(type == StructureType::WALL)
			updateWall(origin, 2);

		return true;
	}

    return false;
}


void TileMap::removeStructureFromMap(int32_t structureId) noexcept
{
	if(auto found = m_structuresById.find(structureId); found != m_structuresById.end())
	{
		auto view = m_registry.view<Structure>();
		const auto entity = found->second;
		const auto& data = view.get<Structure>(entity);
		const auto type = data.type;

		{// structure area
			sf::Vector2i size;

			switch (type)
			{
				case StructureType::SLAB_1x1:          size = { 1, 1 }; break;
				case StructureType::PALACE:            size = { 3, 3 }; break;
				case StructureType::VEHICLE:           size = { 3, 2 }; break;
				case StructureType::HIGH_TECH:         size = { 2, 2 }; break;
				case StructureType::CONSTRUCTION_YARD: size = { 2, 2 }; break;
				case StructureType::WIND_TRAP:         size = { 2, 2 }; break;
				case StructureType::BARRACKS:          size = { 2, 2 }; break;
				case StructureType::STARPORT:          size = { 3, 3 }; break;
				case StructureType::REFINERY:          size = { 3, 2 }; break;
				case StructureType::REPAIR:            size = { 3, 2 }; break;
				case StructureType::WALL:              size = { 1, 1 }; break;
				case StructureType::TURRET:            size = { 1, 1 }; break;
				case StructureType::ROCKET_TURRET:     size = { 1, 1 }; break;
				case StructureType::SILO:              size = { 2, 2 }; break;
				case StructureType::OUTPOST:           size = { 2, 2 }; break;

				default: break;
			}

			if(size == sf::Vector2i(0, 0))
				return;

			char* mask = m_tileMask.data();
			int32_t offset = structureId;

			for (int32_t i = 0; i < size.y; ++i)
			{
				for (int32_t j = 0; j < size.x; ++j)
				{
					mask[offset + j] = 'R';
					m_structuresById.erase(offset + j);
				}
	
				offset += m_mapSizeInTiles.x;
			}
    	}

		m_registry.destroy(entity);
		m_structuresById.erase(found);
	}
}


std::optional<entt::entity> TileMap::getEntityUnderCursor(const sf::Vector2i& point) noexcept
{
//  check out of bounds
	if(point.x < 1)                       return std::nullopt;
	if(point.y < 1)                       return std::nullopt;
	if(point.x > m_mapSizeInPixels.x - 1) return std::nullopt;
	if(point.y > m_mapSizeInPixels.y - 1) return std::nullopt;
//  cast to cell coords
	int32_t cellX = point.x >> 5;
	int32_t cellY = point.y >> 5;

	const int32_t origin = cellY * m_mapSizeInTiles.x + cellX;

	if(auto found = m_structuresById.find(origin); found != m_structuresById.end())
		return std::make_optional<entt::entity>(found->second);

	return std::nullopt;
}


const std::vector<TileMap::Object>& TileMap::getObjects() const noexcept
{
	return m_objects;
}


std::string_view TileMap::getTileMask() const noexcept
{
	return m_tileMask;
}


const sf::Vector2i& TileMap::getMapSizeInTiles()  const noexcept
{
	return m_mapSizeInTiles;
}


const sf::Vector2i& TileMap::getMapSizeInPixels() const noexcept
{
	return m_mapSizeInPixels;
}


const sf::Vector2i& TileMap::getTileSize() const noexcept
{
	return m_tileSize;
}


bool TileMap::loadLayers(const void* map_node) noexcept
{
	const auto xml_node = static_cast<const rapidxml::xml_node<char>*>(map_node);

	std::vector<TileMap::Tileset> tilesets;
	loadTilesets(map_node, tilesets);

	if(tilesets.empty()) 
		return false;

	auto pMapW  = xml_node->first_attribute("width");
	auto pMapH  = xml_node->first_attribute("height");
	auto pTileW = xml_node->first_attribute("tilewidth");
	auto pTileH = xml_node->first_attribute("tileheight");

	const int32_t map_width   = pMapW  ? std::atoi(pMapW->value())  : 0;
	const int32_t map_height  = pMapH  ? std::atoi(pMapH->value())  : 0;
	const int32_t tile_width  = pTileW ? std::atoi(pTileW->value()) : 0;
	const int32_t tile_height = pTileH ? std::atoi(pTileH->value()) : 0;

	if (!(map_width && map_height && tile_width && tile_height))
		return false;

	m_mapSizeInTiles  = { map_width,  map_height  };
	m_mapSizeInPixels = { map_width * tile_width,  map_height * tile_height  };
	m_tileSize        = { tile_width, tile_height };

	m_tileMask.resize(static_cast<size_t>(map_width * map_height), 'S');

	for (auto layer_node = xml_node->first_node("layer");
		      layer_node != nullptr;
		      layer_node = layer_node->next_sibling("layer"))
	{
		auto name = layer_node->first_attribute("name");
		std::string layer_name = name ? name->value() : std::string();

		if(layer_name.empty())
			return false;

		if(auto data_node = layer_node->first_node("data"); data_node != nullptr)
		{
			std::vector<int> parsed_layer;
			std::string data(data_node->value());

			std::replace(data.begin(), data.end(), ',', ' ');

			std::stringstream sstream(data);
			{
				int32_t tile_num = 0;

				while(sstream >> tile_num)
					parsed_layer.push_back(tile_num);
			}

			if(parsed_layer.empty())
				return false;

			const auto bounds = std::minmax_element(parsed_layer.begin(), parsed_layer.end());
			const int32_t min_tile = *bounds.first;
			const int32_t max_tile = *bounds.second;

			auto current_tileset = std::find_if(tilesets.begin(), tilesets.end(),
				[min_tile, max_tile](const TileMap::Tileset& ts)
				{
					return min_tile <= ts.firstGID && max_tile <= ts.firstGID + ts.tileCount;
				});

			if(current_tileset == tilesets.end())
				return false;
				
			if(layer_name == "Landscape")
			{
				if( ! loadLandscape(*current_tileset, parsed_layer)) return false;
				continue;
			}	

			if(layer_name == "Structures")	
				loadStructures(*current_tileset, parsed_layer);
		}
	}

	return true;
}


bool TileMap::loadObjects(const void* map_node) noexcept
{
	const auto xml_node = static_cast<const rapidxml::xml_node<char>*>(map_node);

	for (auto objectGroupNode = xml_node->first_node("objectgroup");
		      objectGroupNode != nullptr;
		      objectGroupNode = objectGroupNode->next_sibling("objectgroup"))
	{
		for (auto objectNode = objectGroupNode->first_node("object");
			      objectNode != nullptr;
			      objectNode = objectNode->next_sibling("object"))
		{
			auto& tme_object = m_objects.emplace_back();

			for (auto attr = objectNode->first_attribute(); attr != nullptr; attr = attr->next_attribute())
			{
				if (strcmp(attr->name(), "x")      == 0) { tme_object.bounds.position.x = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "y")      == 0) { tme_object.bounds.position.y = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "width")  == 0) { tme_object.bounds.size.x     = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "height") == 0) { tme_object.bounds.size.y     = std::atoi(attr->value()); continue; }

				if (strcmp(attr->name(), "name") == 0)  tme_object.name = attr->value();
				if (strcmp(attr->name(), "class") == 0) tme_object.type = attr->value();
			}

			if (const auto propertiesNode = objectNode->first_node("properties"); propertiesNode != nullptr)
			{
				for (auto propertyNode = propertiesNode->first_node("property");
					      propertyNode != nullptr;
					      propertyNode = propertyNode->next_sibling("property"))
				{
					auto& prop = tme_object.properties.emplace_back();

					for (auto attr = propertyNode->first_attribute(); attr != nullptr; attr = attr->next_attribute())
					{
						if (strcmp(attr->name(), "name") == 0) { prop.name  = attr->value(); continue; }
						if (strcmp(attr->name(), "type") == 0) { prop.type  = attr->value(); continue; }
						if (strcmp(attr->name(), "value") == 0)  prop.value = attr->value();
					}
				}
			}
		}
	}

	if(!m_objects.empty())
		return true;
	
	return false;
}


void TileMap::loadTilesets(const void* map_node, std::vector<TileMap::Tileset>& tilesets) noexcept
{
	const auto xml_node = static_cast<const rapidxml::xml_node<char>*>(map_node);

	for (auto tilesetNode = xml_node->first_node("tileset");
			  tilesetNode != nullptr;
			  tilesetNode = tilesetNode->next_sibling("tileset"))
	{
		auto image  = tilesetNode->first_node("image");
		auto source = image->first_attribute("source");

		std::string tex_name = source ? source->value() : std::string();

		if (tex_name.empty())
			continue;

		if (size_t last_slash_pos = tex_name.find_last_of('/'); last_slash_pos != std::string::npos)
			tex_name.erase(0, last_slash_pos + 1);

		sf::Texture* tileset = m_assets.get<sf::Texture>(tex_name);

		if (tileset == nullptr)
			continue;

		auto tile_count = tilesetNode->first_attribute("tilecount");
		auto columns    = tilesetNode->first_attribute("columns");
		auto firstGID   = tilesetNode->first_attribute("firstgid");

		if(tile_count && columns && firstGID)
		{
			Tileset& ts = tilesets.emplace_back();

			ts.texture   = tileset;
			ts.tileCount = (tile_count != nullptr) ? std::atoi(tile_count->value()) : 0;
			ts.columns   = (columns    != nullptr) ? std::atoi(columns->value())    : 0;
			ts.firstGID  = (firstGID   != nullptr) ? std::atoi(firstGID->value())   : 0;
		}
	}
}


bool TileMap::loadLandscape(const Tileset& tileset, const std::vector<int>& parsed_layer) noexcept
{
	std::vector<sf::Vertex> vertices;
	vertices.reserve(parsed_layer.size() * 6);

//  Cached variables
	const int32_t map_width   = m_mapSizeInTiles.x;
	const int32_t map_height  = m_mapSizeInTiles.y;
	const int32_t tile_width  = m_tileSize.x;
	const int32_t tile_height = m_tileSize.y;
	const int32_t columns     = tileset.columns;
	const int32_t firstGID    = tileset.firstGID;
	size_t index = 0;

	for (int32_t y = 0; y < map_height; ++y)
		for (int32_t x = 0; x < map_width; ++x)
		{
			const int32_t tile_id = parsed_layer[index];
			m_tileMask[index] = convertTileNumToChar(tile_id);
			++index;

//  Vertex XY coords				
			const float vX = static_cast<float>(x * tile_width);
			const float vY = static_cast<float>(y * tile_height);

//  Left-top coords of the tile in texture grid
			const int32_t tile_num = tile_id - firstGID;
			const int32_t top = (tile_num >= columns) ? tile_num / columns : 0;
			const int32_t left = tile_num % columns;
			const sf::Vector2f point(left * tile_width, top * tile_height);

//  First triangle
			vertices.emplace_back(sf::Vector2f(vX, vY), sf::Color::White, point);
			vertices.emplace_back(sf::Vector2f(vX + tile_width, vY), sf::Color::White, sf::Vector2f(point.x + tile_width, point.y));
			vertices.emplace_back(sf::Vector2f(vX + tile_width, vY + tile_height), sf::Color::White, sf::Vector2f(point.x + tile_width, point.y + tile_height));
//  Second triangle
			vertices.emplace_back(sf::Vector2f(vX, vY), sf::Color::White, point);
			vertices.emplace_back(sf::Vector2f(vX + tile_width, vY + tile_height), sf::Color::White, sf::Vector2f(point.x + tile_width, point.y + tile_height));
			vertices.emplace_back(sf::Vector2f(vX, vY + tile_height), sf::Color::White, sf::Vector2f(point.x, point.y + tile_height));
		}

//  Unload to VBO
	if (!vertices.empty())
	{
		m_texture = tileset.texture;
		m_vertices.setUsage(sf::VertexBuffer::Usage::Static);
		m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);

		if (m_vertices.create(vertices.size()))
			if(m_vertices.update(vertices.data()))
			return true;
	}

	return false;
}


void TileMap::loadStructures(const Tileset& tileset, const std::vector<int>& parsed_layer) noexcept
{
	auto get_structure_type = [](int32_t tile_num)
	{
		switch (tile_num)
		{
			case 111: return StructureType::WALL;
			case 112: return StructureType::WALL;
			case 113: return StructureType::WALL;
			case 114: return StructureType::WALL;
			case 115: return StructureType::WALL;
			case 116: return StructureType::WALL;
			case 117: return StructureType::WALL;
			case 118: return StructureType::WALL;
			case 119: return StructureType::WALL;
			case 120: return StructureType::WALL;
			case 121: return StructureType::WALL;
			case 122: return StructureType::WALL;
			case 124: return StructureType::REFINERY;
			case 127: return StructureType::CONSTRUCTION_YARD;
			case 129: return StructureType::WIND_TRAP;
			case 131: return StructureType::OUTPOST;
			case 133: return StructureType::SILO;
			case 135: return StructureType::VEHICLE;
			case 159: return StructureType::BARRACKS;
			case 161: return StructureType::PALACE;
			case 164: return StructureType::HIGH_TECH;
			case 166: return StructureType::REPAIR;
			case 191: return StructureType::SLAB_1x1;
			case 207: return StructureType::STARPORT;
			case 261: return StructureType::TURRET;
			case 269: return StructureType::ROCKET_TURRET;
		
			default: return StructureType::INVALID;
		}
	};

//  Cached variables
	const int32_t map_width  = m_mapSizeInTiles.x;
	const int32_t map_height = m_mapSizeInTiles.y;

	for (int32_t y = 0; y < map_height; ++y)
		for (int32_t x = 0; x < map_width; ++x)
		{
			const int32_t index = y * map_width + x;
			const int32_t tile_id = parsed_layer[index];

			if (tile_id)
			{
				m_tileMask[index] = convertTileNumToChar(tile_id);

				auto bld_type = get_structure_type(tile_id);

				if(bld_type < StructureType::MAX)
				{
					putStructureOnMap(bld_type, x, y);
				}
			}
		}
}


char TileMap::convertTileNumToChar(int32_t index) const noexcept
{
#if defined(__GNUC__) || defined(__MINGW32__)

	switch (index)
	{
//      rocky soil
		case 1 ... 5:     return 'R';
		case 13 ... 16:   return 'R';
		case 25 ... 27:   return 'R';
		case 31 ... 34:   return 'R';
		case 40 ... 42:   return 'R';
		case 45:          return 'R';
		case 48:          return 'R';
		case 55:          return 'R';
		case 61:          return 'R'; 
		case 82:          return 'R';
		case 83:          return 'R';
		case 101:         return 'R';
//      sandy soil
		case 6 ... 12:    return 'S';
		case 17 ... 24:   return 'S';	
		case 28 ... 30:   return 'S';
		case 35 ... 39:   return 'S';
		case 43:          return 'S';
		case 44:          return 'S';
		case 46:          return 'S';
		case 47:          return 'S';
		case 49 ... 54:   return 'S';
		case 56 ... 60:   return 'S';
		case 62 ... 81:   return 'S';
		case 84 ... 100:  return 'S';
		case 102 ... 109: return 'S';
//      NOTE: Tiles occupied by buildings will be filled in later when loaded by the Builder class
//      Wall
		case 111 ... 122: return 'R';
//      Building
		case 124 ... 137: return 'R';
		case 140 ... 153: return 'R';
		case 159 ... 165: return 'R';
		case 175 ... 181: return 'R';
		case 192 ... 195: return 'R';
		case 207 ... 209: return 'R';
		case 223 ... 225: return 'R';
		case 239 ... 241: return 'R';	
		case 255 ... 286: return 'R';
//      Concrete slab
		case 191:         return 'R';
//      sandy soil by default
		default:          return 'S';
	}

#elif defined(_MSC_VER)

	switch (index)
	{
//      rocky soil
		case 1:   return 'R';
		case 2:   return 'R';
		case 3:   return 'R';
		case 4:   return 'R';
		case 5:   return 'R';
		case 13:  return 'R';
		case 14:  return 'R';
		case 15:  return 'R';
		case 16:  return 'R';
		case 25:  return 'R';
		case 26:  return 'R';
		case 27:  return 'R';
		case 31:  return 'R';
		case 32:  return 'R';
		case 33:  return 'R';
		case 34:  return 'R';
		case 40:  return 'R';
		case 41:  return 'R';
		case 42:  return 'R';
		case 45:  return 'R';
		case 48:  return 'R';
		case 55:  return 'R';
		case 61:  return 'R'; 
		case 82:  return 'R';
		case 83:  return 'R';
		case 101: return 'R';
//      sandy soil
		case 6:   return 'S';
		case 7:   return 'S';
		case 8:   return 'S';
		case 9:   return 'S';
		case 10:  return 'S';
		case 11:  return 'S';
		case 12:  return 'S';
		case 17:  return 'S';
		case 18:  return 'S';
		case 19:  return 'S';
		case 20:  return 'S';
		case 21:  return 'S';
		case 22:  return 'S';
		case 23:  return 'S';
		case 24:  return 'S';
		case 28:  return 'S';
		case 29:  return 'S';
		case 30:  return 'S';
		case 35:  return 'S';
		case 36:  return 'S';
		case 37:  return 'S';
		case 38:  return 'S';
		case 39:  return 'S';
		case 43:  return 'S';
		case 44:  return 'S';
		case 46:  return 'S';
		case 47:  return 'S';
		case 49:  return 'S';
		case 50:  return 'S';
		case 51:  return 'S';
		case 52:  return 'S';
		case 53:  return 'S';
		case 54:  return 'S';
		case 56:  return 'S';
		case 57:  return 'S';
		case 58:  return 'S';
		case 59:  return 'S';
		case 60:  return 'S';
		case 62:  return 'S';
		case 63:  return 'S';
		case 64:  return 'S';
		case 65:  return 'S';
		case 66:  return 'S';
		case 67:  return 'S';
		case 68:  return 'S';
		case 69:  return 'S';
		case 70:  return 'S';
		case 71:  return 'S';
		case 72:  return 'S';
		case 73:  return 'S';
		case 74:  return 'S';
		case 75:  return 'S';
		case 76:  return 'S';
		case 77:  return 'S';
		case 78:  return 'S';
		case 79:  return 'S';
		case 80:  return 'S';
		case 81:  return 'S';
		case 84:  return 'S';
		case 85:  return 'S';
		case 86:  return 'S';
		case 87:  return 'S';
		case 88:  return 'S';
		case 89:  return 'S';
		case 90:  return 'S';
		case 91:  return 'S';
		case 92:  return 'S';
		case 93:  return 'S';
		case 94:  return 'S';
		case 95:  return 'S';
		case 96:  return 'S';
		case 97:  return 'S';
		case 98:  return 'S';
		case 99:  return 'S';
		case 100: return 'S';
		case 102: return 'S';
		case 103: return 'S';
		case 104: return 'S';
		case 105: return 'S';
		case 106: return 'S';
		case 107: return 'S';
		case 108: return 'S';
		case 109: return 'S';
//      Wall
		case 111: return 'R';
		case 112: return 'R';
		case 113: return 'R';
		case 114: return 'R';
		case 115: return 'R';
		case 116: return 'R';
		case 117: return 'R';
		case 118: return 'R';
		case 119: return 'R';
		case 120: return 'R';
		case 121: return 'R';
		case 122: return 'R';
		case 124: return 'R';
		case 125: return 'R';
		case 126: return 'R';
		case 127: return 'R';
		case 128: return 'R';
		case 129: return 'R';
		case 130: return 'R';
		case 131: return 'R';
		case 132: return 'R';
		case 133: return 'R';
		case 134: return 'R';
		case 135: return 'R';
		case 136: return 'R';
		case 137: return 'R';
		case 140: return 'R';
		case 141: return 'R';
		case 142: return 'R';
		case 143: return 'R';
		case 144: return 'R';
		case 145: return 'R';
		case 146: return 'R';
		case 147: return 'R';
		case 148: return 'R';
		case 149: return 'R';
		case 150: return 'R';
		case 151: return 'R';
		case 152: return 'R';
		case 153: return 'R';
		case 159: return 'R';
		case 160: return 'R';
		case 161: return 'R';
		case 162: return 'R';
		case 163: return 'R';
		case 164: return 'R';
		case 165: return 'R';
		case 175: return 'R';
		case 176: return 'R';
		case 177: return 'R';
		case 178: return 'R';
		case 179: return 'R';
		case 180: return 'R';
		case 181: return 'R';
		case 192: return 'R';
		case 193: return 'R';
		case 194: return 'R';
		case 195: return 'R';
		case 207: return 'R';
		case 208: return 'R';
		case 209: return 'R';
		case 223: return 'R';
		case 224: return 'R';
		case 225: return 'R';
		case 239: return 'R';
		case 240: return 'R';
		case 241: return 'R';
		case 255: return 'R';
		case 256: return 'R';
		case 257: return 'R';
		case 258: return 'R';
		case 259: return 'R';
		case 260: return 'R';
		case 261: return 'R';
		case 262: return 'R';
		case 263: return 'R';
		case 264: return 'R';
		case 265: return 'R';
		case 266: return 'R';
		case 267: return 'R';
		case 268: return 'R';
		case 269: return 'R';
		case 270: return 'R';
		case 271: return 'R';
		case 272: return 'R';
		case 273: return 'R';
		case 274: return 'R';
		case 275: return 'R';
		case 276: return 'R';
		case 277: return 'R';
		case 278: return 'R';
		case 279: return 'R';
		case 280: return 'R';
		case 281: return 'R';
		case 282: return 'R';
		case 283: return 'R';
		case 284: return 'R';
		case 285: return 'R';
		case 286: return 'R';
//      Concrete slab
		case 191: return 'R';
//      sandy soil by default
		default:  return 'S';
	}

#else
    #error "Unsupported compiler"
#endif
}


void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(m_texture)
	{
		states.texture = m_texture;
		target.draw(m_vertices, states);
	}
}


void TileMap::updateWall(int32_t origin, int32_t level) noexcept
{
    if(level > 0)
	{
		int32_t left   = origin - 1;
		int32_t top    = origin - m_mapSizeInTiles.x;
		int32_t right  = origin + 1;
		int32_t bottom = origin + m_mapSizeInTiles.x;

		const char* field = m_tileMask.data();

		bool a = (left >= 0)                  ? (field[left]   == 'W') : false;
		bool b = (top >= 0)                   ? (field[top]    == 'W') : false;
		bool c = (right < m_tileMask.size())  ? (field[right]  == 'W') : false;
		bool d = (bottom < m_tileMask.size()) ? (field[bottom] == 'W') : false;

		const auto tex_coords = getTexCoordsOf(computeWallType(a, b, c, d));
		const auto entity = m_structuresById[origin];

		auto view = m_registry.view<Tile>();
		auto& sprite = view.get<Tile>(entity);
		sprite.setTextureRect(tex_coords);

		if(a) updateWall(left, level - 1);
		if(b) updateWall(top, level - 1);
		if(c) updateWall(right, level - 1);
		if(d) updateWall(bottom, level - 1);
	}
}


TileMap::WallCellType TileMap::computeWallType(bool left, bool top, bool right, bool bottom) noexcept
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


sf::IntRect TileMap::getTexCoordsOf(TileMap::WallCellType type) noexcept
{
    switch (type)
    {
        case WallCellType::DOT:               return { {0,   0}, {32, 32} }; 
        case WallCellType::LEFT_RIGHT:        return { {32,  0}, {32, 32} }; 
        case WallCellType::BOTTOM_TOP:        return { {64,  0}, {32, 32} }; 
        case WallCellType::TOP_RIGHT:         return { {96,  0}, {32, 32} }; 
        case WallCellType::RIGHT_BOTTOM:      return { {128, 0}, {32, 32} };
        case WallCellType::BOTTOM_LEFT:       return { {160, 0}, {32, 32} };
        case WallCellType::LEFT_TOP:          return { {192, 0}, {32, 32} };
        case WallCellType::TOP_RIGHT_BOTTOM:  return { {224, 0}, {32, 32} }; 
        case WallCellType::RIGHT_BOTTOM_LEFT: return { {256, 0}, {32, 32} }; 
        case WallCellType::BOTTOM_LEFT_TOP:   return { {288, 0}, {32, 32} };
        case WallCellType::LEFT_TOP_RIGHT:    return { {320, 0}, {32, 32} };
        case WallCellType::CROSS:             return { {352, 0}, {32, 32} };
    
        default: return { {0, 0}, {32, 32} };
    }
}


sf::IntRect TileMap::getTexCoordsOf(StructureType type) const noexcept
{
	switch (type)
	{
		case StructureType::SLAB_1x1:          return { {0,   160}, {32, 32} };
		case StructureType::PALACE:            return { {64,  96 }, {96, 96} };
		case StructureType::VEHICLE:           return { {256, 32 }, {96, 64} };
		case StructureType::HIGH_TECH:         return { {160, 96 }, {64, 64} };
		case StructureType::CONSTRUCTION_YARD: return { {0,   32 }, {64, 64} };
		case StructureType::WIND_TRAP:         return { {64,  32 }, {64, 64} };
		case StructureType::BARRACKS:          return { {0,   96 }, {64, 64} };
		case StructureType::STARPORT:          return { {0,   192}, {96, 96} };
		case StructureType::REFINERY:          return { {416, 0  }, {96, 64} };
		case StructureType::REPAIR:            return { {224, 96 }, {96, 64} };
		case StructureType::WALL:              return { {0,   0  }, {32, 32} };
		case StructureType::TURRET:            return { {192, 288}, {32, 32} };
		case StructureType::ROCKET_TURRET:     return { {448, 288}, {32, 32} };
		case StructureType::SILO:              return { {192, 32 }, {64, 64} };
		case StructureType::OUTPOST:           return { {128, 32 }, {64, 64} };

		default: return sf::IntRect();
	}
}


sf::IntRect TileMap::getBoundsOf(StructureType type, int32_t coordX, int32_t coordY) const noexcept
{
	switch (type)
	{
		case StructureType::SLAB_1x1:          return { {coordX, coordY}, {32, 32} };
		case StructureType::PALACE:            return { {coordX, coordY}, {96, 96} };
		case StructureType::VEHICLE:           return { {coordX, coordY}, {96, 64} };
		case StructureType::HIGH_TECH:         return { {coordX, coordY}, {64, 64} };
		case StructureType::CONSTRUCTION_YARD: return { {coordX, coordY}, {64, 64} };
		case StructureType::WIND_TRAP:         return { {coordX, coordY}, {64, 64} };
		case StructureType::BARRACKS:          return { {coordX, coordY}, {64, 64} };
		case StructureType::STARPORT:          return { {coordX, coordY}, {96, 96} };
		case StructureType::REFINERY:          return { {coordX, coordY}, {96, 64} };
		case StructureType::REPAIR:            return { {coordX, coordY}, {96, 64} };
		case StructureType::WALL:              return { {coordX, coordY}, {32, 32} };
		case StructureType::TURRET:            return { {coordX, coordY}, {32, 32} };
		case StructureType::ROCKET_TURRET:     return { {coordX, coordY}, {32, 32} };
		case StructureType::SILO:              return { {coordX, coordY}, {64, 64} };
		case StructureType::OUTPOST:           return { {coordX, coordY}, {64, 64} }; 

		default: return sf::IntRect();
	}
}


int32_t TileMap::getHitPointsOf(StructureType type) const noexcept
{
	switch (type)
	{
		case StructureType::SLAB_1x1:          return 40;
		case StructureType::PALACE:            return 2000;
		case StructureType::VEHICLE:           return 800;
		case StructureType::HIGH_TECH:         return 1000;
		case StructureType::CONSTRUCTION_YARD: return 800;
		case StructureType::WIND_TRAP:         return 400;
		case StructureType::BARRACKS:          return 600;
		case StructureType::STARPORT:          return 1000;
		case StructureType::REFINERY:          return 900;
		case StructureType::REPAIR:            return 1800;
		case StructureType::WALL:              return 140;
		case StructureType::TURRET:            return 025;
		case StructureType::ROCKET_TURRET:     return 500;
		case StructureType::SILO:              return 300;
		case StructureType::OUTPOST:           return 1000; 

		default: return 1;
	}
}