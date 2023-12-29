#include <sstream>
#include <algorithm>
#include <cstring>

#include "rapidxml_utils.hpp"

#include "utils/FileProvider.hpp"
#include "managers/AssetManager.hpp"
#include "scenes/battle/TileMap.hpp"

bool TileMap::loadFromFile(const std::filesystem::path& fPath) noexcept
{
//  Make sure it hasn't been downloaded before
	m_layers.clear();
	m_objects.clear();
	m_collisionMask.clear();
	m_title.clear();

	if (fPath.empty())
		return false;

	if(fPath.extension().generic_string() != ".tmx")
		return false;

	m_title = fPath.stem().string();

	auto pDocument = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(fPath.string().c_str());
	pDocument->parse<0>(xmlFile.data());

	const auto mapNode = pDocument->first_node("map");

	if (!mapNode)
		return nullptr;

	return ( loadLayers(mapNode) && loadObjects(mapNode) );
}

bool TileMap::loadLayers(const rapidxml::xml_node<char>* mapNode) noexcept
{
	std::vector<TilesetData> tilesets = parseTilesets(mapNode);

	auto pMapW  = mapNode->first_attribute("width");
	auto pMapH  = mapNode->first_attribute("height");
	auto pTileW = mapNode->first_attribute("tilewidth");
	auto pTileH = mapNode->first_attribute("tileheight");

	const std::int32_t map_width   = pMapW  ? std::atoi(pMapW->value())  : 0;
	const std::int32_t map_height  = pMapH  ? std::atoi(pMapH->value())  : 0;
	const std::int32_t tile_width  = pTileW ? std::atoi(pTileW->value()) : 0;
	const std::int32_t tile_height = pTileH ? std::atoi(pTileH->value()) : 0;

	if (!(map_width && map_height && tile_width && tile_height))
		return false;

	m_mapSize  = { map_width,  map_height };
	m_tileSize = { tile_width, tile_height };

	for (auto layerNode = mapNode->first_node("layer");
		      layerNode != nullptr;
		      layerNode = layerNode->next_sibling("layer"))
	{
		auto name = layerNode->first_attribute("name");
		std::string layerName = name ? name->value() : std::string();

		if (layerName.empty())
			continue;

		auto dataNode = layerNode->first_node("data");

		if (!dataNode)
			continue;

		std::vector<std::int32_t> parsed_layer = parseCSVstring(dataNode);

		if (parsed_layer.empty())
			continue;

		const auto bounds = std::minmax_element(parsed_layer.begin(), parsed_layer.end());
		std::int32_t minTile = *bounds.first;
		std::int32_t maxTile = *bounds.second;

		auto currentTileset = std::find_if(tilesets.begin(), tilesets.end(),
		[minTile, maxTile](const TilesetData& ts)
		{
			return minTile <= ts.firstGID && maxTile <= ts.firstGID + ts.tileCount;
		});

		if (currentTileset == tilesets.end())
			continue;

//  Find which render plan the layer belongs to
		Layer* layer = &m_layers.emplace_back();
		layer->name = layerName;
		layer->texture = currentTileset->texture;

		std::vector<sf::Vertex> vertices;
		vertices.reserve(std::count_if(parsed_layer.begin(), parsed_layer.end(),
			[](std::int32_t n) { return n > 0; }));

		for (std::int32_t y = 0; y < map_height; ++y)
			for (std::int32_t x = 0; x < map_width; ++x)
			{
				std::int32_t tile_id = parsed_layer[y * map_width + x];

				if (tile_id)
				{
//  Find the sequence tile number in this tileset
					std::int32_t tile_num = tile_id - currentTileset->firstGID;

					std::int32_t Y = (tile_num >= currentTileset->columns) ? tile_num / currentTileset->columns : 0;
					std::int32_t X = tile_num % currentTileset->columns;

					std::int32_t offsetX = X * tile_width;
					std::int32_t offsetY = Y * tile_height;

//  Left-top coords of the tile in texture grid
					std::int32_t top = (tile_num >= currentTileset->columns) ? tile_num / currentTileset->columns : 0;
					std::int32_t left = tile_num % currentTileset->columns;
					sf::Vector2f point(left * tile_width, top * tile_height);

//  First triangle
					vertices.emplace_back(sf::Vector2f(x * tile_width, y * tile_height), point);
					vertices.emplace_back(sf::Vector2f(x * tile_width + tile_width, y * tile_height), sf::Vector2f(point.x + tile_width, point.y));
					vertices.emplace_back(sf::Vector2f(x * tile_width + tile_width, y * tile_height + tile_height), sf::Vector2f(point.x + tile_width, point.y + tile_height));
//  Second triangle
					vertices.emplace_back(sf::Vector2f(x * tile_width, y * tile_height), point);
					vertices.emplace_back(sf::Vector2f(x * tile_width + tile_width, y * tile_height + tile_height), sf::Vector2f(point.x + tile_width, point.y + tile_height));
					vertices.emplace_back(sf::Vector2f(x * tile_width, y * tile_height + tile_height), sf::Vector2f(point.x, point.y + tile_height));
				}
			}

		if (!vertices.empty())
		{
			layer->vertexBuffer.create(vertices.size());
			layer->vertexBuffer.update(vertices.data());
			layer->vertexBuffer.setUsage(sf::VertexBuffer::Static);
			layer->vertexBuffer.setPrimitiveType(sf::Triangles);
		}
	}

	return ( ! m_layers.empty() );
}

bool TileMap::loadObjects(const rapidxml::xml_node<char>* mapNode) noexcept
{
	for (auto objectGroupNode = mapNode->first_node("objectgroup");
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

	return !m_objects.empty();
}

std::vector<TileMap::TilesetData> TileMap::parseTilesets(const rapidxml::xml_node<char>* mapNode) noexcept
{
	std::vector<TileMap::TilesetData> tilesets;

	for (auto tilesetNode = mapNode->first_node("tileset");
			  tilesetNode != nullptr;
			  tilesetNode = tilesetNode->next_sibling("tileset"))
	{
		auto image = tilesetNode->first_node("image");
		auto source = image->first_attribute("source");

		std::string texName = source ? source->value() : std::string();

		if (texName.empty())
			continue;

		if (std::size_t last_slash_pos = texName.find_last_of('/'); last_slash_pos != std::string::npos)
			texName.erase(0, last_slash_pos + 1);

		sf::Texture* tileset = AssetManager::get<sf::Texture>(texName);

		if (tileset == nullptr)
			continue;

		TilesetData& ts = tilesets.emplace_back();

		auto tileCount = tilesetNode->first_attribute("tilecount");
		auto columns   = tilesetNode->first_attribute("columns");
		auto firstGID  = tilesetNode->first_attribute("firstgid");

		ts.texture   = tileset;
		ts.tileCount = tileCount ? std::atoi(tileCount->value()) : 0;
		ts.columns   = columns ? std::atoi(columns->value()) : 0;
		ts.rows      = (tileCount || !columns) ? 0 : ts.tileCount / ts.columns;
		ts.firstGID  = firstGID ? std::atoi(firstGID->value()) : 0;
	}

	return tilesets;
}

std::vector<std::int32_t> TileMap::parseCSVstring(const rapidxml::xml_node<char>* dataNode) noexcept
{
	std::string data(dataNode->value());
	std::replace(data.begin(), data.end(), ',', ' ');

	std::vector<std::int32_t> parsed_layer;

	std::stringstream sstream(data);
	{
		std::int32_t tile_num = 0;

		while (sstream >> tile_num)
			parsed_layer.push_back(tile_num);
	}

	return parsed_layer;

}
