#include <sstream>
#include <algorithm>
#include <cstring>

#include "rapidxml_utils.hpp"

#include "utils/FileProvider.hpp"
#include "loaders/Assets.hpp"
#include "scenes/mission/utils/TileMap.hpp"

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

void TileMap::reset() noexcept
{
	landscape.vertices.~VertexBuffer();
	landscape.texture = nullptr;
	
	staticTiles.vertices.clear();
	staticTiles.texture = nullptr;

	animatedTiles.vertices.clear();
	animatedTiles.texture = nullptr;

	tileMask.clear();
	objects.clear();
	title.clear();

	mapSize  = { 0, 0 };
	tileSize = { 0, 0 };
}

bool TileMap::loadLayers(const rapidxml::xml_node<char>* map_node) noexcept
{
	std::vector<TileMap::TilesetData> tilesets;
	parseTilesets(map_node, tilesets);

	if(tilesets.empty()) 
		return false;

	auto pMapW  = map_node->first_attribute("width");
	auto pMapH  = map_node->first_attribute("height");
	auto pTileW = map_node->first_attribute("tilewidth");
	auto pTileH = map_node->first_attribute("tileheight");

	const std::int32_t map_width   = pMapW  ? std::atoi(pMapW->value())  : 0;
	const std::int32_t map_height  = pMapH  ? std::atoi(pMapH->value())  : 0;
	const std::int32_t tile_width  = pTileW ? std::atoi(pTileW->value()) : 0;
	const std::int32_t tile_height = pTileH ? std::atoi(pTileH->value()) : 0;

	if (!(map_width && map_height && tile_width && tile_height))
		return false;

	mapSize  = { map_width,  map_height  };
	tileSize = { tile_width, tile_height };
	tileMask.resize(static_cast<std::size_t>(map_width * map_height), ' ');

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
			std::vector<std::int32_t> parsed_layer;
			std::string data(data_node->value());

			std::replace(data.begin(), data.end(), ',', ' ');

			std::stringstream sstream(data);
			{
				std::int32_t tile_num = 0;

				while(sstream >> tile_num)
					parsed_layer.push_back(tile_num);
			}

			if(parsed_layer.empty())
				return false;

			const auto bounds = std::minmax_element(parsed_layer.begin(), parsed_layer.end());
			const std::int32_t min_tile = *bounds.first;
			const std::int32_t max_tile = *bounds.second;

			auto current_tileset = std::find_if(tilesets.begin(), tilesets.end(),
				[min_tile, max_tile](const TileMap::TilesetData& ts)
				{
					return min_tile <= ts.firstGID && max_tile <= ts.firstGID + ts.tileCount;
				});

			if(current_tileset == tilesets.end())
				return false;
				
			if(layer_name == "Landscape")	
				parseLandscape(*current_tileset, parsed_layer);
			
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

void TileMap::parseTilesets(const rapidxml::xml_node<char>* map_node, std::vector<TileMap::TilesetData>& tilesets) noexcept
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

		if (std::size_t last_slash_pos = tex_name.find_last_of('/'); last_slash_pos != std::string::npos)
			tex_name.erase(0, last_slash_pos + 1);

		sf::Texture* tileset = Assets::instance()->getTexture(tex_name);

		if (tileset == nullptr)
			continue;

		auto tile_count = tilesetNode->first_attribute("tilecount");
		auto columns    = tilesetNode->first_attribute("columns");
		auto firstGID   = tilesetNode->first_attribute("firstgid");

		if(tile_count && columns && firstGID)
		{
			TilesetData& ts = tilesets.emplace_back();

			ts.texture   = tileset;
			ts.tileCount = tile_count ? std::atoi(tile_count->value()) : 0;
			ts.columns   = columns ? std::atoi(columns->value()) : 0;
			ts.firstGID  = firstGID ? std::atoi(firstGID->value()) : 0;
		}
	}
}

void TileMap::parseLandscape(const TilesetData& td, const std::vector<std::int32_t>& parsed_layer) noexcept
{
	std::vector<sf::Vertex> vertices;
	vertices.reserve(std::count_if(parsed_layer.begin(), parsed_layer.end(),
		[](std::int32_t n) { return n > 0; }));

//  Cached variables
	const std::int32_t map_width   = mapSize.x;
	const std::int32_t map_height  = mapSize.y;
	const std::int32_t tile_width  = tileSize.x;
	const std::int32_t tile_height = tileSize.y;
	const std::int32_t columns     = td.columns;
	const std::int32_t firstGID    = td.firstGID;

	for (std::int32_t y = 0; y < map_height; ++y)
		for (std::int32_t x = 0; x < map_width; ++x)
		{
			const std::int32_t tile_id = parsed_layer[y * map_width + x];

//  Vertex XY coords				
			const float cX = static_cast<float>(x * tile_width);
			const float cY = static_cast<float>(y * tile_height);

//  Left-top coords of the tile in texture grid
			const std::int32_t tile_num = tile_id - firstGID;
			const std::int32_t top = (tile_num >= columns) ? tile_num / columns : 0;
			const std::int32_t left = tile_num % columns;
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
		landscape.texture = td.texture;
		landscape.vertices.setUsage(sf::VertexBuffer::Static);
		landscape.vertices.setPrimitiveType(sf::Triangles);
		landscape.vertices.create(vertices.size());
		landscape.vertices.update(vertices.data());
	}
}

void TileMap::parseBuildings(const TilesetData& td, const std::vector<std::int32_t>& parsed_layer) noexcept
{
	std::vector<sf::Vertex> vertices;
	vertices.reserve(std::count_if(parsed_layer.begin(), parsed_layer.end(),
		[](std::int32_t n) { return n > 0; }));

//  Cached variables
	const std::int32_t map_width   = mapSize.x;
	const std::int32_t map_height  = mapSize.y;
	const std::int32_t tile_width  = tileSize.x;
	const std::int32_t tile_height = tileSize.y;
	const std::int32_t columns     = td.columns;
	const std::int32_t firstGID    = td.firstGID;
	const std::int32_t tile_count  = td.tileCount;

	for (std::int32_t y = 0; y < map_height; ++y)
		for (std::int32_t x = 0; x < map_width; ++x)
		{
			std::int32_t tile_id = parsed_layer[y * map_width + x];

			if (tile_id)
			{
//  Vertex XY coords				
				const std::int32_t tile_num = tile_id - firstGID;
				float cX = static_cast<float>(x * tile_width);
				float cY = static_cast<float>(y * tile_height);

//  Left-top coords of the tile in texture grid
				std::int32_t top = (tile_num >= columns) ? tile_num / columns : 0;
				std::int32_t left = tile_num % columns;
				sf::Vector2f point(left * tile_width, top * tile_height);

//  First triangle
				vertices.emplace_back(sf::Vector2f(cX, cY), point);
				vertices.emplace_back(sf::Vector2f(cX + tile_width, cY), sf::Vector2f(point.x + tile_width, point.y));
				vertices.emplace_back(sf::Vector2f(cX + tile_width, cY + tile_height), sf::Vector2f(point.x + tile_width, point.y + tile_height));
//  Second triangle
				vertices.emplace_back(sf::Vector2f(cX, cY), point);
				vertices.emplace_back(sf::Vector2f(cX + tile_width, cY + tile_height), sf::Vector2f(point.x + tile_width, point.y + tile_height));
				vertices.emplace_back(sf::Vector2f(cX, cY + tile_height), sf::Vector2f(point.x, point.y + tile_height));
			}
		}
}

char TileMap::getTileID(std::int32_t index) const noexcept
{
	switch (index)
	{
	case 1: return 'R';  // rocky soil
	case 17:  
	case 18: 
	case 24: 
	case 38:
	case 43:
	case 47:
	case 49:
	case 53:
	case 62:
	case 70:
	case 74:
	case 76:
	case 81:
	case 89:
	case 99: return 'S';  // sandy soil
	case 111: 
	case 112:
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
	case 119:
	case 120: return 'W'; // Wall

	default:
		break;
	}

	return 'S'; // sandy soil by default
}