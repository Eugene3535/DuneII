#include <sstream>
#include <algorithm>
#include <cstring>

#include "rapidxml_utils.hpp"

#include "utils/FileProvider.hpp"
#include "loaders/Assets.hpp"
#include "scenes/mission/tilemap/TileMap.hpp"

bool TileMap::loadFromFile(const std::filesystem::path& file_path) noexcept
{
//  Make sure it hasn't been downloaded before
	reset();

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

Building* TileMap::placeBuilding(int32_t x, int32_t y, Building::Type type) noexcept
{
	auto setupTilesOnMask = [](char** mask, int32_t x, int32_t y, int32_t width, int32_t height)
	{
		for (int32_t i = 0; i < height; ++i)
			for (int32_t j = 0; j < width; ++j)
				mask[x + j][y + i] = 'B';
	};

	if(x < 0 || y < 0)
		return nullptr;

	Building::Data data = {};
	data.type = type;

	int32_t coordX = (x << 5);
	int32_t coordY = (y << 5);

	if(auto texture = Assets::instance()->getTexture("Buildings.png"); texture != nullptr)
	{
		switch (type)
		{
			case Building::CONCRETE_SLAB:
			{
				return nullptr;
				// data.textureRect = { 0, 160, 32, 32 };
				// data.globalBounds = { coordX, coordY, 32, 32 };
				// data.cost = 5;
				// data.hitPoints = 40; 
				// data.maxHitPoints = 40;
				// setupTilesOnMask(collisionMask.data(), x, y, 1, 1); // need to replace 'B' to 'C'
			}
			break;

			case Building::CONSTRUCTION_YARD:
			{
				data.textureRect = { 0, 32, 64, 64 };
				data.globalBounds = { coordX, coordY, 64, 64 };
				data.cost = 900;
				data.hitPoints = 400; 
				data.maxHitPoints = 400;
				setupTilesOnMask(collisionMask.data(), x, y, 2, 2);
			}
			break;

			case Building::SPICE_SILO:
			{
				data.textureRect = { 192, 32, 64, 64 };
				data.globalBounds = { coordX, coordY, 64, 64 };
				data.cost = 300;
				data.hitPoints = 150; 
				data.maxHitPoints = 150;
				setupTilesOnMask(collisionMask.data(), x, y, 2, 2);
			}
			break;

			case Building::STARPORT:
			{
				data.textureRect = { 0, 196, 96, 96 };
				data.globalBounds = { coordX, coordY, 96, 96 };
				data.cost = 500;
				data.hitPoints = 1000; 
				data.maxHitPoints = 1000;
				setupTilesOnMask(collisionMask.data(), x, y, 3, 3);
			} 
			break;

			case Building::WIND_TRAP:
			{
				data.textureRect = { 64, 32, 64, 64 };
				data.globalBounds = { coordX, coordY, 64, 64 };
				data.cost = 300;
				data.hitPoints = 400; 
				data.maxHitPoints = 400;
				setupTilesOnMask(collisionMask.data(), x, y, 2, 2);
			}
			break;

			case Building::SPICE_REFINERY:
			{
				data.textureRect = { 416, 0, 96, 64 };
				data.globalBounds = { coordX, coordY, 96, 64 };
				data.cost = 300;
				data.hitPoints = 400; 
				data.maxHitPoints = 400;
				setupTilesOnMask(collisionMask.data(), x, y, 3, 2);
			}
			break;

			case Building::RADAR_OUTPOST:
			{
				data.textureRect = { 128, 32, 64, 64 };
				data.globalBounds = { coordX, coordY, 64, 64 };
				data.cost = 400;
				data.hitPoints = 1000; 
				data.maxHitPoints = 1000;
				setupTilesOnMask(collisionMask.data(), x, y, 2, 2);
			}
			break;

			case Building::REPAIR_FACILITY: break;

			case Building::PALACE:
			{
				data.textureRect = { 64, 96, 96, 96 };
				data.globalBounds = { coordX, coordY, 96, 96 };
				data.cost = 999;
				data.hitPoints = 2000; 
				data.maxHitPoints = 2000;
				setupTilesOnMask(collisionMask.data(), x, y, 3, 3);
			}
			break;

			case Building::HIGH_TECH_FACILITY:
			{
				data.textureRect = { 160, 96, 64, 64 };
				data.globalBounds = { coordX, coordY, 64, 64 };
				data.cost = 500;
				data.hitPoints = 1000; 
				data.maxHitPoints = 1000;
				setupTilesOnMask(collisionMask.data(), x, y, 2, 2);
			}
			break;

			case Building::BARRACKS:
			{
				data.textureRect = { 0, 96, 64, 64 };
				data.globalBounds = { coordX, coordY, 64, 64 };
				data.cost = 300;
				data.hitPoints = 600; 
				data.maxHitPoints = 600;
				setupTilesOnMask(collisionMask.data(), x, y, 2, 2);
			}
			break;

			case Building::VEHICLE_FACTORY:
			{
				data.textureRect = { 256, 32, 96, 64 };
				data.globalBounds = { coordX, coordY, 96, 64 };
				data.cost = 400;
				data.hitPoints = 800; 
				data.maxHitPoints = 800;
				setupTilesOnMask(collisionMask.data(), x, y, 3, 2);
			}
			break;

			case Building::WALL:
			{
				data.textureRect = { 0, 0, 32, 32 };
				data.globalBounds = { coordX, coordY, 32, 32 };
				data.cost = 50;
				data.hitPoints = 140; 
				data.maxHitPoints = 140;
				setupTilesOnMask(collisionMask.data(), x, y, 1, 1);
			}
			break;

			case Building::TURRET:
			{
				data.textureRect = { 192, 288, 32, 32 };
				data.globalBounds = { coordX, coordY, 32, 32 };
				data.cost = 125;
				data.hitPoints = 250; 
				data.maxHitPoints = 250;
				setupTilesOnMask(collisionMask.data(), x, y, 1, 1);
			}
			break;

			case Building::ROCKET_TURRET:
			{
				data.textureRect = { 448, 288, 32, 32 };
				data.globalBounds = { coordX, coordY, 32, 32 };
				data.cost = 250;
				data.hitPoints = 500; 
				data.maxHitPoints = 500;
				setupTilesOnMask(collisionMask.data(), x, y, 1, 1);
			}
			break;
		}

		if((data.globalBounds.left + data.textureRect.width) >= mapSizeInPixels.x || (data.globalBounds.top + data.textureRect.height) >= mapSizeInPixels.y)
			return nullptr;

		auto bld = buildings.emplace_back(std::make_unique<Building>()).get();
		bld->setTexture(*texture);
		bld->setTextureRect(data.textureRect);
		bld->setPosition(static_cast<float>(coordX), static_cast<float>(coordY));
		bld->construct(&data);	

		return bld;
	}

	return nullptr;
}

void TileMap::reset() noexcept
{
	landscape.vertices.~VertexBuffer();
	landscape.texture = nullptr;

	objects.clear();
	tileMask.clear();
	collisionMask.clear();
	title.clear();

	mapSizeInTiles  = { 0, 0 };
	mapSizeInPixels  = { 0, 0 };
	tileSize = { 0, 0 };
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
			std::vector<int32_t> parsed_layer;
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
			auto& tme_object = objects.emplace_back();

			for (auto attr = objectNode->first_attribute(); attr != nullptr; attr = attr->next_attribute())
			{
				if (strcmp(attr->name(), "x")      == 0) { tme_object.position.x = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "y")      == 0) { tme_object.position.y = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "width")  == 0) { tme_object.size.x     = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "height") == 0) { tme_object.size.y     = std::atoi(attr->value()); continue; }

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

	return !objects.empty();
}

void TileMap::parseTilesets(const rapidxml::xml_node<char>* map_node, std::vector<TileMap::Tileset>& tilesets) noexcept
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

		sf::Texture* tileset = Assets::instance()->getTexture(tex_name);

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

void TileMap::parseLandscape(const Tileset& tileset, const std::vector<int32_t>& parsed_layer) noexcept
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

void TileMap::parseBuildings(const Tileset& tileset, const std::vector<int32_t>& parsed_layer) noexcept
{
	auto get_building_type = [](int32_t tile_num)
	{
		switch (tile_num)
		{
			case 111 ... 120: return Building::Type::WALL;
			case 124: return Building::Type::SPICE_REFINERY;
			case 127: return Building::Type::CONSTRUCTION_YARD;
			case 129: return Building::Type::WIND_TRAP;
			case 131: return Building::Type::RADAR_OUTPOST;
			case 133: return Building::Type::SPICE_SILO;
			case 135: return Building::Type::VEHICLE_FACTORY;
			case 159: return Building::Type::BARRACKS;
			case 161: return Building::Type::PALACE;
			case 164: return Building::Type::HIGH_TECH_FACILITY;
			case 207: return Building::Type::STARPORT;
			case 261: return Building::Type::TURRET;
			case 269: return Building::Type::ROCKET_TURRET;
		
			default: return Building::Type::NONE;
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
					placeBuilding(x, y, bld_type);
				}
			}
		}
}

// See https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II
int8_t TileMap::convertTileNumToChar(int32_t index) const noexcept
{
	switch (index)
	{
		case 1 ... 5:
		case 13 ... 16:
		case 25 ... 27:
		case 31 ... 34: 
		case 40 ... 42:
		case 45:
		case 48:
		case 55: 
		case 61: 
		case 82:
		case 83:
		case 101: return 'R';  // rocky soil

		case 6 ... 12:
		case 17 ... 24: 	
		case 28 ... 30:
		case 35 ... 39:
		case 43:
		case 44:
		case 46:
		case 47:
		case 49 ... 54:
		case 56 ... 60:
		case 62 ... 81:
		case 84 ... 100:
		case 102 ... 109: return 'S';  // sandy soil

		case 111 ... 120: return 'W'; // Wall

		case 124 ... 137:
		case 140 ... 153:
		case 159 ... 165:
		case 175 ... 181:
		case 192 ... 195:
		case 207 ... 209:
		case 223 ... 225:
		case 239 ... 241:	
		case 255 ... 286: return 'B'; // Building

		case 191: return 'C'; // Concrete slab

		default: return 'S'; // sandy soil by default
	}
}