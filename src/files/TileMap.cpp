#include <array>
#include <cstring>
#include <algorithm>
#include <charconv>

#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_utils.hpp>
#include <magic_enum/magic_enum.hpp>

#include "common/info/GameInfo.hpp"
#include "files/TileMap.hpp"


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
	reset();

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

		m_tileMask.resize(mapWidth * mapHeight);

		m_mapSize  = { mapWidth,  mapHeight  };
		m_tileSize = { tileWidth, tileHeight };

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


void TileMap::reset() noexcept
{
	m_title.clear();
	m_vertices.clear();
	m_indices.clear();
	m_objects.clear();
	m_tileMask.clear();
	m_mapSize = glms_ivec2_zero();
	m_tileSize = glms_ivec2_zero();
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


std::string_view TileMap::getTileMask() const noexcept
{
	return m_tileMask;
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
			tileset.tileCount = (tileCount != nullptr)  ? atoi(tileCount->value())            : 0;
			tileset.columns   = (columns != nullptr)    ? atoi(columns->value())              : 0;
			tileset.firstGID  = (firstGID != nullptr)   ? atoi(firstGID->value())             : 0;
			tileset.rows      = (tileset.tileCount > 0) ? tileset.tileCount / tileset.columns : 0;

			++index;
		}
	}

	auto parse_csv = [](const void* node, std::vector<int>& result) -> void
	{
		const auto dataNode = static_cast<const rapidxml::xml_node<char>*>(node);
		std::string csv(dataNode->value(), dataNode->value_size());

		const auto commaCount = std::ranges::count_if(csv, [](const char c) { return c == ','; });
		result.reserve(commaCount + 1);

		const char* begin = csv.data();
		const char* end = begin + csv.size();

		while (begin < end) 
		{
			while (begin < end && *begin == ' ') ++begin;

			int value = 0;
			auto [p, ec] = std::from_chars(begin, end, value);

			if (ec == std::errc()) 
			{
				result.push_back(value);
				begin = p;
			}
			else ++begin;

			while (begin < end && (*begin == ',' || *begin == ' ')) ++begin;
		}
	};

	for (auto layerNode = mapNode->first_node("layer");
			  layerNode != nullptr;
			  layerNode = layerNode->next_sibling("layer"))
	{
		auto dataNode = layerNode->first_node("data");

		if (!dataNode)
			continue;

		std::vector<int> tileIDs;
		parse_csv(dataNode, tileIDs);

		const auto minMaxElems = std::minmax_element(tileIDs.begin(), tileIDs.end());
		const int minTile = *minMaxElems.first;
		const int maxTile = *minMaxElems.second;

		auto currentTileset = std::find_if(tilesets.begin(), tilesets.end(),
		[minTile, maxTile](const Tileset& ts)
		{
			return minTile <= ts.firstGID && maxTile <= (ts.firstGID + ts.tileCount);
		});

		if (currentTileset == tilesets.end())
			continue;

		if (auto attrName = layerNode->first_attribute("name"))
		{
			if(strcmp(attrName->value(), "Landscape") == 0)
				loadLandscape(*currentTileset, tileIDs);

			if(strcmp(attrName->value(), "Structures") == 0)
				loadStructures(*currentTileset, tileIDs);
		}
	}

	return (!m_vertices.empty());
}


bool TileMap::loadObjects(const void* rootNode) noexcept
{
	const auto mapNode = static_cast<const rapidxml::xml_node<char>*>(rootNode);

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
				if (strcmp(attribute->name(), "x") == 0)      { object.coords.x = atoi(attribute->value()); continue; }
				if (strcmp(attribute->name(), "y") == 0)      { object.coords.y = atoi(attribute->value()); continue; }
				if (strcmp(attribute->name(), "width") == 0)  { object.size.x   = atoi(attribute->value()); continue; }
				if (strcmp(attribute->name(), "height") == 0) { object.size.y   = atoi(attribute->value()); continue; }
				if (strcmp(attribute->name(), "name")   == 0)   object.name     = { attribute->value(), attribute->value_size() };
				if (strcmp(attribute->name(), "class")  == 0)   object.type     = { attribute->value(), attribute->value_size() };
			}

			if (const auto propertiesNode = objectNode->first_node("properties"); propertiesNode != nullptr)
			{
				for (auto propertyNode = propertiesNode->first_node("property");
					      propertyNode != nullptr;
					      propertyNode = propertyNode->next_sibling("property"))
				{
					auto& property = object.properties.emplace_back();

                    std::string_view type;
                    std::string_view value;

					for (auto attribute = propertyNode->first_attribute(); attribute; attribute = attribute->next_attribute())
					{
						if (strcmp(attribute->name(), "name") == 0) 
                            property.name = { attribute->value(), attribute->value_size() }; 
                        
						else if (strcmp(attribute->name(), "type") == 0)  
                            type  = { attribute->value(), attribute->value_size() };

						else if (strcmp(attribute->name(), "value") == 0) 
                            value = { attribute->value(), attribute->value_size() };
					}

                    if (type == "bool")
                    {
                        property.value = (value == "true") ? true : false; 
                    }
                    else if (type == "int")
                    {
                        property.value = atoi(value.data()); 
                    }
                    else if (type == "float")
                    {
                        float result = 0.f;
                        auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), result);
                        
                        if (ec == std::errc())
                            property.value = result; 
                    }
                    else if (type == "string")
                    {
                        property.value = std::string(value); 
                    }
				}
			}
		}
	}

	return ( ! m_objects.empty() );
}


void TileMap::loadLandscape(const Tileset& tileset, std::span<const int32_t> tileIds) noexcept
{
	vec2s ratio;
	{
		const int32_t textureWidth  = tileset.columns * m_tileSize.x;
		const int32_t textureHeight = tileset.rows * m_tileSize.y;
		ratio.x = 1.f / textureWidth;
		ratio.y = 1.f / textureHeight;
	}

	const int32_t mapWidth   = m_mapSize.x;
	const int32_t mapHeight  = m_mapSize.y;
	const int32_t tileWidth  = m_tileSize.x;
	const int32_t tileHeight = m_tileSize.x;

	m_vertices.reserve(tileIds.size() * 4);
	m_indices.reserve(tileIds.size() * 6);
	size_t tileIndex = 0;

	for (int32_t y = 0; y < mapHeight; ++y)
	{
		for (int32_t x = 0; x < mapWidth; ++x)
		{
			const int32_t tileID = tileIds[y * mapWidth + x];
			const int32_t tileNum = tileID - tileset.firstGID;

			if (tileID)
			{
				m_tileMask[tileIndex++] = GameInfo::convertTileNumToChar(tileID);

				const int32_t tileY = (tileNum >= tileset.columns) ? tileNum / tileset.columns : 0;
				const int32_t tileX = tileNum % tileset.columns;

				const int32_t positionX = tileX * tileWidth;
				const int32_t positionY = tileY * tileHeight;
//  UV
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
}


void TileMap::loadStructures(const Tileset& tileset, std::span<const int> tileIds) noexcept
{
	const int32_t width  = m_mapSize.x;
	const int32_t height = m_mapSize.y;

	for (int32_t y = 0; y < height; ++y)
	{
		for (int32_t x = 0; x < width; ++x)
		{
			const size_t index = y * width + x;
			const int32_t tileID = tileIds[index];

			if (!tileID)
				continue;
			
			if(const auto type = GameInfo::getStructureType(tileID); type != StructureInfo::Type::Undefined)
			{
				auto& object = m_objects.emplace_back();

				object.name = magic_enum::enum_name(type);
				object.type = "Structure";
				object.coords = { x, y };
				object.size = GameInfo::getStructureSizeInTiles(type);
			}
		}
	}
}