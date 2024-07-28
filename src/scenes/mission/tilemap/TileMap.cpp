#include <sstream>
#include <algorithm>
#include <cstring>

#include "rapidxml_utils.hpp"

#include "common/FileProvider.hpp"
#include "managers/assets/AssetManager.hpp"
#include "scenes/mission/tilemap/TileMap.hpp"

bool TileMap::loadFromFile(const std::filesystem::path& file_path) noexcept
{
//  Make sure it hasn't been downloaded before
	unload();

	if (file_path.empty())
		return false;

	if(file_path.extension().string() != ".tmx")
		return false;

	title = file_path.stem().string();

	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(file_path.string().c_str());
	document->parse<0>(xmlFile.data());

	const auto map_node = document->first_node("map");

	if (!map_node)
		return false;

	if(loadLayers(map_node))
		if(loadObjects(map_node))
			return true;

	return false;
}

int32_t TileMap::costOf(Building::Type type) const noexcept
{
    switch (type)
    {
        case Building::CONCRETE_SLAB:      return 5;
        case Building::CONSTRUCTION_YARD:  return 900;
        case Building::SPICE_SILO:         return 300;
        case Building::STARPORT:           return 500;
        case Building::WIND_TRAP:          return 300;
        case Building::SPICE_REFINERY:     return 300;
        case Building::RADAR_OUTPOST:      return 400;
        case Building::REPAIR_FACILITY:    return 700;
        case Building::PALACE:             return 999;
        case Building::HIGH_TECH_FACILITY: return 500;
        case Building::BARRACKS:           return 300;
        case Building::VEHICLE_FACTORY:    return 400;
        case Building::WALL:               return 50;
        case Building::TURRET:             return 125;
        case Building::ROCKET_TURRET:      return 250;    

        default: return 0;
    }
}

Building* TileMap::placeBuilding(Building::Type type, int32_t cellX, int32_t cellY) noexcept
{
	if(cellX < 0 || cellY < 0)
		return nullptr;

	int32_t coordX = (cellX << 5);
	int32_t coordY = (cellY << 5);
	int32_t origin = cellY * mapSizeInTiles.x + cellX;

	if(auto found = m_buildings.find(origin); found == m_buildings.end())
	{
		if(const auto texture = Assets->getTexture("Buildings.png"); texture != nullptr)
		{
			auto building = &m_buildings[origin];

			building->m_type = type;
			building->setTexture(texture);
			building->setTextureRect(getTexCoordsOf(type));
			building->setPosition(coordX, coordY);
			building->m_hitPoints = building->m_maxHitPoints = getHitPointsOf(type);
			building->m_bounds = getBoundsOf(type, coordX, coordY);

			auto setupTilesOnMask = [](char** mask, int32_t x, int32_t y, int32_t width, int32_t height, char symbol = 'B')
			{
				for (int32_t i = 0; i < height; ++i)
					for (int32_t j = 0; j < width; ++j)
						mask[y + i][x + j] = symbol;
			};

			switch (type)
			{
				case Building::CONCRETE_SLAB:      setupTilesOnMask(collisionMask.data(), cellX, cellY, 1, 1, 'C'); break;
				case Building::CONSTRUCTION_YARD:  setupTilesOnMask(collisionMask.data(), cellX, cellY, 2, 2);      break;
				case Building::SPICE_SILO:         setupTilesOnMask(collisionMask.data(), cellX, cellY, 2, 2);      break;
				case Building::STARPORT:           setupTilesOnMask(collisionMask.data(), cellX, cellY, 3, 3);      break;
				case Building::WIND_TRAP:          setupTilesOnMask(collisionMask.data(), cellX, cellY, 2, 2);      break;
				case Building::SPICE_REFINERY:     setupTilesOnMask(collisionMask.data(), cellX, cellY, 3, 2);      break;
				case Building::RADAR_OUTPOST:      setupTilesOnMask(collisionMask.data(), cellX, cellY, 2, 2);      break;
				case Building::REPAIR_FACILITY:    setupTilesOnMask(collisionMask.data(), cellX, cellY, 3, 2);      break;
				case Building::PALACE:             setupTilesOnMask(collisionMask.data(), cellX, cellY, 3, 3);      break;
				case Building::HIGH_TECH_FACILITY: setupTilesOnMask(collisionMask.data(), cellX, cellY, 2, 2);      break;
				case Building::BARRACKS:           setupTilesOnMask(collisionMask.data(), cellX, cellY, 2, 2);      break;
				case Building::VEHICLE_FACTORY:    setupTilesOnMask(collisionMask.data(), cellX, cellY, 3, 2);      break;
				case Building::WALL:               setupTilesOnMask(collisionMask.data(), cellX, cellY, 1, 1, 'W'); break;
				case Building::TURRET:             setupTilesOnMask(collisionMask.data(), cellX, cellY, 1, 1);      break;
				case Building::ROCKET_TURRET:      setupTilesOnMask(collisionMask.data(), cellX, cellY, 1, 1);      break;

				default: break;
			}
				
			return building;
		}
	}

	return nullptr;
}

void TileMap::eraseBuilding(const Building* building) noexcept
{
	//m_buildings.returnObjectBack(building);
}

std::vector<Building*> TileMap::getAllBuildings() noexcept
{
	std::vector<Building*> blds;

	for(auto& [id, building] : m_buildings)
		blds.push_back(&building);

	return blds; //m_buildings.getOccupiedObjects();
}

void TileMap::unload() noexcept
{
	landscape.vertices.~VertexBuffer();
	landscape.texture = nullptr;

	m_objects.clear();
	tileMask.clear();
	collisionMask.clear();
	title.clear();

	mapSizeInTiles  = { 0, 0 };
	mapSizeInPixels  = { 0, 0 };
	tileSize = { 0, 0 };
}

const std::vector<TileMap::Object>& TileMap::getObjects() const noexcept
{
	return m_objects;
}

bool TileMap::loadLayers(const rapidxml::xml_node<char>* map_node) noexcept
{
	std::vector<TileMap::Tileset> tilesets;
	parseTilesets(map_node, tilesets);

	if(tilesets.empty()) 
		return false;

	auto pMapW  = map_node->first_attribute("width");
	auto pMapH  = map_node->first_attribute("height");
	auto pTileW = map_node->first_attribute("tilewidth");
	auto pTileH = map_node->first_attribute("tileheight");

	const int32_t map_width   = pMapW  ? std::atoi(pMapW->value())  : 0;
	const int32_t map_height  = pMapH  ? std::atoi(pMapH->value())  : 0;
	const int32_t tile_width  = pTileW ? std::atoi(pTileW->value()) : 0;
	const int32_t tile_height = pTileH ? std::atoi(pTileH->value()) : 0;

	if (!(map_width && map_height && tile_width && tile_height))
		return false;

	mapSizeInTiles  = { map_width,  map_height  };
	mapSizeInPixels = { map_width * tile_width,  map_height * tile_height  };
	tileSize        = { tile_width, tile_height };

	tileMask.resize(static_cast<size_t>(map_width * map_height), 'S');
	collisionMask.resize(static_cast<size_t>(map_height), nullptr);

	for (size_t i = 0; i < map_height; ++i)
		collisionMask[i] = tileMask.data() + i * map_width;

	for (auto layer_node = map_node->first_node("layer");
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
				parseLandscape(*current_tileset, parsed_layer);
				continue;
			}	

			if(layer_name == "Buildings")	
				parseBuildings(*current_tileset, parsed_layer);
		}
	}

	return true;
}

bool TileMap::loadObjects(const rapidxml::xml_node<char>* map_node) noexcept
{
	for (auto objectGroupNode = map_node->first_node("objectgroup");
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
				if (strcmp(attr->name(), "x")      == 0) { tme_object.bounds.left   = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "y")      == 0) { tme_object.bounds.top    = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "width")  == 0) { tme_object.bounds.width  = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "height") == 0) { tme_object.bounds.height = std::atoi(attr->value()); continue; }

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
	{
		auto get_area_of = [this](House houseName) -> sf::IntRect
		{
			for(auto& object : m_objects)
			{
				if(object.name == "Area")
				{
					auto found = std::find_if(object.properties.begin(), object.properties.end(), [houseName](const TileMap::Object::Property& property)
					{
						return houseName == static_cast<House>(std::stoul(property.value));
					});

					if(found != object.properties.end())
						return object.bounds;
				}
			}

			return sf::IntRect();
		};

		auto atreides_area  = get_area_of(House::Atreides);
		auto ordos_area     = get_area_of(House::Ordos);
		auto harkonnen_area = get_area_of(House::Harkonnen);

		auto buildings = getAllBuildings();

		for(auto b : buildings)
		{
			auto type = b->getType();
			const bool repairable = ((type!= Building::CONCRETE_SLAB) && (type != Building::WALL));

			if(repairable)
			{
				if(b->getBounds().intersects(atreides_area))
				{
					b->changeOwner(House::Atreides);
				}
				else if(b->getBounds().intersects(ordos_area))
				{
					b->changeOwner(House::Ordos);
				}
				else if(b->getBounds().intersects(harkonnen_area))
				{
					b->changeOwner(House::Harkonnen);
				}
			}
		}

		return true;
	}

	return false;
}

void TileMap::parseTilesets(const rapidxml::xml_node<>* map_node, std::vector<TileMap::Tileset>& tilesets) noexcept
{
	for (auto tilesetNode = map_node->first_node("tileset");
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

		sf::Texture* tileset = Assets->getTexture(tex_name);

		if (tileset == nullptr)
			continue;

		auto tile_count = tilesetNode->first_attribute("tilecount");
		auto columns    = tilesetNode->first_attribute("columns");
		auto firstGID   = tilesetNode->first_attribute("firstgid");

		if(tile_count && columns && firstGID)
		{
			Tileset& ts = tilesets.emplace_back();

			ts.texture   = tileset;
			ts.tileCount = tile_count ? std::atoi(tile_count->value()) : 0;
			ts.columns   = columns ? std::atoi(columns->value()) : 0;
			ts.firstGID  = firstGID ? std::atoi(firstGID->value()) : 0;
		}
	}
}

void TileMap::parseLandscape(const Tileset& tileset, const std::vector<int>& parsed_layer) noexcept
{
	std::vector<sf::Vertex> vertices;
	vertices.reserve(parsed_layer.size());

//  Cached variables
	const int32_t map_width   = mapSizeInTiles.x;
	const int32_t map_height  = mapSizeInTiles.y;
	const int32_t tile_width  = tileSize.x;
	const int32_t tile_height = tileSize.y;
	const int32_t columns     = tileset.columns;
	const int32_t firstGID    = tileset.firstGID;

	for (int32_t y = 0; y < map_height; ++y)
		for (int32_t x = 0; x < map_width; ++x)
		{
			const int32_t index = y * map_width + x;
			const int32_t tile_id = parsed_layer[index];
			tileMask[index] = convertTileNumToChar(tile_id);

//  Vertex XY coords				
			const float cX = static_cast<float>(x * tile_width);
			const float cY = static_cast<float>(y * tile_height);

//  Left-top coords of the tile in texture grid
			const int32_t tile_num = tile_id - firstGID;
			const int32_t top = (tile_num >= columns) ? tile_num / columns : 0;
			const int32_t left = tile_num % columns;
			const sf::Vector2f point(left * tile_width, top * tile_height);

//  First triangle
			vertices.emplace_back(sf::Vector2f(cX, cY), point);
			vertices.emplace_back(sf::Vector2f(cX + tile_width, cY), sf::Vector2f(point.x + tile_width, point.y));
			vertices.emplace_back(sf::Vector2f(cX + tile_width, cY + tile_height), sf::Vector2f(point.x + tile_width, point.y + tile_height));
//  Second triangle
			vertices.emplace_back(sf::Vector2f(cX, cY), point);
			vertices.emplace_back(sf::Vector2f(cX + tile_width, cY + tile_height), sf::Vector2f(point.x + tile_width, point.y + tile_height));
			vertices.emplace_back(sf::Vector2f(cX, cY + tile_height), sf::Vector2f(point.x, point.y + tile_height));
		}

//  Unload to VBO
	if (!vertices.empty())
	{
		landscape.texture = tileset.texture;
		landscape.vertices.setUsage(sf::VertexBuffer::Static);
		landscape.vertices.setPrimitiveType(sf::Triangles);
		landscape.vertices.create(vertices.size());
		landscape.vertices.update(vertices.data());
	}
}

void TileMap::parseBuildings(const Tileset& tileset, const std::vector<int>& parsed_layer) noexcept
{
	auto get_building_type = [](int32_t tile_num)
	{
		switch (tile_num)
		{
			case 111 ... 122: return Building::WALL;
			case 124: return Building::SPICE_REFINERY;
			case 127: return Building::CONSTRUCTION_YARD;
			case 129: return Building::WIND_TRAP;
			case 131: return Building::RADAR_OUTPOST;
			case 133: return Building::SPICE_SILO;
			case 135: return Building::VEHICLE_FACTORY;
			case 159: return Building::BARRACKS;
			case 161: return Building::PALACE;
			case 164: return Building::HIGH_TECH_FACILITY;
			case 166: return Building::REPAIR_FACILITY;
			case 207: return Building::STARPORT;
			case 261: return Building::TURRET;
			case 269: return Building::ROCKET_TURRET;
		
			default: return Building::NONE;
		}
	};

//  Cached variables
	const int32_t map_width   = mapSizeInTiles.x;
	const int32_t map_height  = mapSizeInTiles.y;
	const int32_t tile_width  = tileSize.x;
	const int32_t tile_height = tileSize.y;
	const int32_t columns     = tileset.columns;
	const int32_t firstGID    = tileset.firstGID;
	const int32_t tile_count  = tileset.tileCount;

	for (int32_t y = 0; y < map_height; ++y)
		for (int32_t x = 0; x < map_width; ++x)
		{
			const int32_t index = y * map_width + x;
			const int32_t tile_id = parsed_layer[index];

			if (tile_id)
			{
				tileMask[index] = convertTileNumToChar(tile_id);

				auto bld_type = get_building_type(tile_id);

				if(bld_type != Building::Type::NONE)
				{
					placeBuilding(bld_type, x, y);
				}
			}
		}
}

char TileMap::convertTileNumToChar(int32_t index) const noexcept
{
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
//      Wall
		case 111 ... 122: return 'W';
//      Building
		case 124 ... 137: return 'B';
		case 140 ... 153: return 'B';
		case 159 ... 165: return 'B';
		case 175 ... 181: return 'B';
		case 192 ... 195: return 'B';
		case 207 ... 209: return 'B';
		case 223 ... 225: return 'B';
		case 239 ... 241: return 'B';	
		case 255 ... 286: return 'B';
//      Concrete slab
		case 191:         return 'C';

		default:          return 'S'; // sandy soil by default
	}
}

sf::IntRect TileMap::getTexCoordsOf(Building::Type type) const noexcept
{
    switch (type)
    {
        case Building::CONCRETE_SLAB:      return { 0, 160, 32, 32   };
        case Building::CONSTRUCTION_YARD:  return { 0, 32, 64, 64    };
        case Building::SPICE_SILO:         return { 192, 32, 64, 64  };
        case Building::STARPORT:           return { 0, 192, 96, 96   };
        case Building::WIND_TRAP:          return { 64, 32, 64, 64   };
        case Building::SPICE_REFINERY:     return { 416, 0, 96, 64   };
        case Building::RADAR_OUTPOST:      return { 128, 32, 64, 64  };
        case Building::REPAIR_FACILITY:    return { 224, 96, 96, 64  };
        case Building::PALACE:             return { 64, 96, 96, 96   };
        case Building::HIGH_TECH_FACILITY: return { 160, 96, 64, 64  };
        case Building::BARRACKS:           return { 0, 96, 64, 64    };
        case Building::VEHICLE_FACTORY:    return { 256, 32, 96, 64  };
        case Building::WALL:               return { 0, 0, 32, 32     };
        case Building::TURRET:             return { 192, 288, 32, 32 };
        case Building::ROCKET_TURRET:      return { 448, 288, 32, 32 };

        default: return sf::IntRect();
    }
}

sf::IntRect TileMap::getBoundsOf(Building::Type type, int32_t coordX, int32_t coordY) const noexcept
{
	switch (type)
	{
		case Building::CONCRETE_SLAB:      return { coordX, coordY, 32, 32 };
		case Building::CONSTRUCTION_YARD:  return { coordX, coordY, 64, 64 };
		case Building::SPICE_SILO:         return { coordX, coordY, 64, 64 };
		case Building::STARPORT:           return { coordX, coordY, 96, 96 };
		case Building::WIND_TRAP:          return { coordX, coordY, 64, 64 };
		case Building::SPICE_REFINERY:     return { coordX, coordY, 96, 64 };
		case Building::RADAR_OUTPOST:      return { coordX, coordY, 64, 64 };
		case Building::REPAIR_FACILITY:    return { coordX, coordY, 96, 64 };
		case Building::PALACE:             return { coordX, coordY, 96, 96 };
		case Building::HIGH_TECH_FACILITY: return { coordX, coordY, 64, 64 };
		case Building::BARRACKS:           return { coordX, coordY, 64, 64 };
		case Building::VEHICLE_FACTORY:    return { coordX, coordY, 96, 64 };
		case Building::WALL:               return { coordX, coordY, 32, 32 };
		case Building::TURRET:             return { coordX, coordY, 32, 32 };
		case Building::ROCKET_TURRET:      return { coordX, coordY, 32, 32 };

		default: return sf::IntRect();
	}
}

int32_t TileMap::getHitPointsOf(Building::Type type) const noexcept
{
    switch (type)
    {
        case Building::CONCRETE_SLAB:      return 40;
        case Building::CONSTRUCTION_YARD:  return 800;
        case Building::SPICE_SILO:         return 300;
        case Building::STARPORT:           return 1000;
        case Building::WIND_TRAP:          return 400;
        case Building::SPICE_REFINERY:     return 900;
        case Building::RADAR_OUTPOST:      return 1000;
        case Building::REPAIR_FACILITY:    return 1800;
        case Building::PALACE:             return 2000;
        case Building::HIGH_TECH_FACILITY: return 1000;
        case Building::BARRACKS:           return 600;
        case Building::VEHICLE_FACTORY:    return 800;
        case Building::WALL:               return 140;
        case Building::TURRET:             return 250;
        case Building::ROCKET_TURRET:      return 500;

        default: return 1;
    }
}