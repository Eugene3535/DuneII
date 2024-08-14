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

std::vector<Building*> TileMap::getAllBuildings() noexcept
{
	std::vector<Building*> blds;

	for(auto& [id, building] : m_buildings)
		blds.push_back(&building);

	return blds;
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
		return true;
	
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
	size_t index = 0;

	for (int32_t y = 0; y < map_height; ++y)
		for (int32_t x = 0; x < map_width; ++x)
		{
			const int32_t tile_id = parsed_layer[index];
			tileMask[index] = convertTileNumToChar(tile_id);
			index++;

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
			case 191: return Building::CONCRETE_SLAB;
			case 207: return Building::STARPORT;
			case 261: return Building::TURRET;
			case 269: return Building::ROCKET_TURRET;
		
			default: return Building::NONE;
		}
	};

//  Cached variables
	const int32_t map_width  = mapSizeInTiles.x;
	const int32_t map_height = mapSizeInTiles.y;

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
					TileMap::BuildingData data = { bld_type, x, y };
					m_buildingsOnLoad.emplace_back(data);
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