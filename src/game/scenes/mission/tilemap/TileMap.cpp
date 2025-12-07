#include <array>
#include <cstring>
#include <algorithm>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "game/scenes/mission/tilemap/TileMap.hpp"


#define OBJECT_DATA_BUFFER_SIZE 2048


struct Tileset
{
	int32_t columns   = 0;
	int32_t rows      = 0;
	int32_t tileCount = 0;
	int32_t firstGID  = 1;
};


TileMap::TileMap() noexcept:
	m_mapSize(glms_ivec2_zero()),
	m_tileSize(glms_ivec2_zero())
{

}


bool TileMap::loadFromFile(const std::filesystem::path &filePath) noexcept
{
//  Make sure it hasn't been downloaded before
	m_title.clear();
	m_vertices.clear();
	m_indices.clear();
	m_objects.clear();
	m_objectData.clear();
	m_mapSize = glms_ivec2_zero();
	m_tileSize = glms_ivec2_zero();

	if (filePath.empty())
		return false;

	if(filePath.extension().string() != ".tmx")
		return false;

	m_title = filePath.stem().string();

	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filePath.string().c_str());
	document->parse<0>(xmlFile.data());

	if (const auto mapNode = document->first_node("map"))
	{
		auto mapW  = mapNode->first_attribute("width");
		auto mapH  = mapNode->first_attribute("height");
		auto tileW = mapNode->first_attribute("tilewidth");
		auto tileH = mapNode->first_attribute("tileheight");

		const int mapWidth   = mapW  ? atoi(mapW->value())  : 0;
		const int mapHeight  = mapH  ? atoi(mapH->value())  : 0;
		const int tileWidth  = tileW ? atoi(tileW->value()) : 0;
		const int tileHeight = tileH ? atoi(tileH->value()) : 0;

		if( ! (mapWidth && mapHeight && tileWidth && tileHeight) )
			return false;

		m_mapSize  = { mapWidth,  mapHeight  };
		m_tileSize = { tileWidth, tileHeight };

		m_vertices.reserve(mapWidth * mapHeight * 4);
		m_indices.reserve(mapWidth * mapHeight * 6);
		m_objectData.reserve(OBJECT_DATA_BUFFER_SIZE);

		if(loadLayers(static_cast<const void*>(mapNode)))
			if(loadObjects(static_cast<const void*>(mapNode)))
				return true;
	}

	return false;
}


bool TileMap::loadFromSQLiteDb(const std::filesystem::path& filePath) noexcept
{
	return false;
}


std::string_view TileMap::getTitle() const noexcept
{
	return m_title;
}


std::span<const vec4s> TileMap::getVertices() const noexcept
{
	return m_vertices;
}


std::span<const uint32_t> TileMap::getIndices() const noexcept
{
	return m_indices;
}


std::span<const TileMap::Object> TileMap::getObjects() const noexcept
{
	return m_objects;
}


ivec2s TileMap::getMapSize()  const noexcept
{
	return m_mapSize;
}


ivec2s TileMap::getTileSize() const noexcept
{
	return m_tileSize;
}


bool TileMap::loadLayers(const void* rootNode) noexcept
{
	const auto mapNode = static_cast<const rapidxml::xml_node<char>*>(rootNode);

	std::array<Tileset, 2> tilesets;
	{
		size_t index = 0;

		for (auto tilesetNode = mapNode->first_node("tileset");
				  tilesetNode != nullptr;
				  tilesetNode = tilesetNode->next_sibling("tileset"))
		{
			auto tileCount = tilesetNode->first_attribute("tilecount");
			auto columns   = tilesetNode->first_attribute("columns");
			auto firstGID  = tilesetNode->first_attribute("firstgid");

			Tileset& tileset  = tilesets[index];
			tileset.tileCount = (tileCount != nullptr) ? atoi(tileCount->value()) : 0;
			tileset.columns   = (columns   != nullptr) ? atoi(columns->value())   : 0;
			tileset.firstGID  = (firstGID  != nullptr) ? atoi(firstGID->value())  : 0;
			tileset.rows      = (tileset.tileCount > 0) ? tileset.tileCount / tileset.columns : 0;

			++index;
		}
	}

	for (auto layerNode = mapNode->first_node("layer");
			  layerNode != nullptr;
			  layerNode = layerNode->next_sibling("layer"))
	{
		auto attrName = layerNode->first_attribute("name");
		std::string name = attrName ? attrName->value() : std::string();

		auto dataNode = layerNode->first_node("data");

		if (!dataNode)
			continue;

		std::string csv(dataNode->value());
		std::string token;
		std::istringstream iss(csv);
		std::vector<int> tileIDs;
    
		while (std::getline(iss, token, ','))
			tileIDs.push_back(std::stoi(token));

		const auto minMaxElems = std::minmax_element(tileIDs.begin(), tileIDs.end());
		const int minTile = *minMaxElems.first;
		const int maxTile = *minMaxElems.second;

		auto currentTileset = std::find_if(tilesets.begin(), tilesets.end(),
		[minTile, maxTile](const Tileset& ts)
		{
			return minTile <= ts.firstGID && maxTile <= (ts.firstGID + ts.tileCount);
		});

		if(currentTileset == tilesets.end())
			continue;

		if(auto attrName = layerNode->first_attribute("name"))
		{
			if(strcmp(attrName->value(), "Landscape") == 0)
				loadLandscape(*currentTileset, tileIDs);

			// if(strcmp(attrName->value(), "Structures") == 0)
			// 	loadStructures(*current_tileset, parsed_layer);
		}
	}

	return (!m_vertices.empty());
}


bool TileMap::loadObjects(const void* rootNode) noexcept
{
	const auto mapNode = static_cast<const rapidxml::xml_node<char>*>(rootNode);

	auto write_object_data = [this] (std::string_view& field, const char* data) -> void
	{
		size_t size = m_objectData.size();
		size_t len = strlen(data);
		m_objectData += data;
		field = std::string_view(m_objectData.data() + size, len);

		assert(m_objectData.size() < OBJECT_DATA_BUFFER_SIZE); // buffer pointers will become invalid, need to increase the limit
	};

	for (auto objectGroupNode = mapNode->first_node("objectgroup");
		      objectGroupNode != nullptr;
		      objectGroupNode = objectGroupNode->next_sibling("objectgroup"))
	{
		for (auto objectNode = objectGroupNode->first_node("object");
			      objectNode != nullptr;
			      objectNode = objectNode->next_sibling("object"))
		{
			auto& object = m_objects.emplace_back();

			for (auto attribute = objectNode->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
			{
				if (strcmp(attribute->name(), "x")      == 0) { object.bounds.x = atoi(attribute->value()); continue; }
				if (strcmp(attribute->name(), "y")      == 0) { object.bounds.y = atoi(attribute->value()); continue; }
				if (strcmp(attribute->name(), "width")  == 0) { object.bounds.z = atoi(attribute->value()); continue; }
				if (strcmp(attribute->name(), "height") == 0) { object.bounds.w = atoi(attribute->value()); continue; }

				if (strcmp(attribute->name(), "name") == 0)  write_object_data(object.name, attribute->value());
				if (strcmp(attribute->name(), "class") == 0) write_object_data(object.type, attribute->value());
			}

			if (const auto propertiesNode = objectNode->first_node("properties"); propertiesNode != nullptr)
			{
				for (auto propertyNode = propertiesNode->first_node("property");
					      propertyNode != nullptr;
					      propertyNode = propertyNode->next_sibling("property"))
				{
					auto& property = object.properties.emplace_back();

					for (auto attribute = propertyNode->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
					{
						if (strcmp(attribute->name(), "name") == 0) { write_object_data(property.name,  attribute->value()); continue; }
						if (strcmp(attribute->name(), "type") == 0) { write_object_data(property.type,  attribute->value()); continue; }
						if (strcmp(attribute->name(), "value") == 0)  write_object_data(property.value, attribute->value());
					}
				}
			}
		}
	}
	
	return ( ! m_objects.empty() );
}


void TileMap::loadLandscape(const Tileset& tileset, std::span<const int> tileIds) noexcept
{
	vec2s ratio;
	{
		int textureWidth  = tileset.columns * m_tileSize.x;
		int textureHeight = tileset.rows * m_tileSize.y;
		ratio.x = 1.f / textureWidth;
		ratio.y = 1.f / textureHeight;
	}

	int mapWidth   = m_mapSize.x;
	int mapHeight  = m_mapSize.y;
	int tileWidth  = m_tileSize.x;
	int tileHeight = m_tileSize.x;

	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			const int tileID = tileIds[y * mapWidth + x];
			const int tileNum = tileID - tileset.firstGID;

			const int tileY = (tileNum >= tileset.columns) ? tileNum / tileset.columns : 0;
			const int tileX = tileNum % tileset.columns;

			const int positionX = tileX * tileWidth;
			const int positionY = tileY * tileHeight;
//  Texture coords
			const float left   = positionX * ratio.x;
			const float top    = positionY * ratio.y;
			const float right  = (positionX + tileWidth) * ratio.x;
			const float bottom = (positionY + tileHeight) * ratio.y;
//  Vertices
			const vec2s leftBottom  = { static_cast<float>(x) * tileWidth,              static_cast<float>(y) * tileHeight + tileHeight };
			const vec2s rightBootom = { static_cast<float>(x) * tileWidth + tileWidth,  static_cast<float>(y) * tileHeight + tileHeight };
			const vec2s rightTop    = { static_cast<float>(x) * tileWidth + tileWidth,  static_cast<float>(y) * tileHeight };
			const vec2s leftTop     = { static_cast<float>(x) * tileWidth,              static_cast<float>(y) * tileHeight };

			const uint32_t index = static_cast<uint32_t>(m_vertices.size());

			m_vertices.push_back({ leftBottom.x,  leftBottom.y,  left,  bottom });
			m_vertices.push_back({ rightBootom.x, rightBootom.y, right, bottom });
			m_vertices.push_back({ rightTop.x,    rightTop.y,    right, top    });
			m_vertices.push_back({ leftTop.x,     leftTop.y,     left,  top    });

			m_indices.push_back(index);
			m_indices.push_back(index + 1);
			m_indices.push_back(index + 2);

			m_indices.push_back(index);
			m_indices.push_back(index + 2);
			m_indices.push_back(index + 3);
		}
	}
}