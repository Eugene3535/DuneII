#include <array>
#include <cstring>
#include <algorithm>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "game/scenes/mission/common/Structures.hpp"
#include "game/scenes/mission/loader/TiledMapLoader.hpp"


struct Tileset
{
	int32_t columns   = 0;
	int32_t rows      = 0;
	int32_t tileCount = 0;
	int32_t firstGID  = 1;
};


static char convert_tile_num_to_char(int32_t index) noexcept;

TiledMapLoader::TiledMapLoader() noexcept:
	m_mapSize(glms_ivec2_zero()),
	m_tileSize(glms_ivec2_zero())
{

}


bool TiledMapLoader::loadFromFile(const std::filesystem::path &filePath) noexcept
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

		m_vertices.reserve(mapWidth * mapHeight * 4);
		m_indices.reserve(mapWidth * mapHeight * 6);
		m_tileMask.resize(mapWidth * mapHeight);

		m_mapSize  = { mapWidth,  mapHeight  };
		m_tileSize = { tileWidth, tileHeight };

		if(loadLayers(static_cast<const void*>(mapNode)))
			if(loadObjects(static_cast<const void*>(mapNode)))
				return true;
	}

	return false;
}


bool TiledMapLoader::loadFromSQLiteDb(const std::filesystem::path& filePath) noexcept
{
	return false;
}


void TiledMapLoader::reset() noexcept
{
	m_title.clear();
	m_vertices.clear();
	m_indices.clear();
	m_objects.clear();
	m_tileMask.clear();
	m_mapSize = glms_ivec2_zero();
	m_tileSize = glms_ivec2_zero();
}


std::string_view TiledMapLoader::getTitle() const noexcept
{
	return m_title;
}


std::span<const vec4s> TiledMapLoader::getVertices() const noexcept
{
	return m_vertices;
}


std::span<const uint32_t> TiledMapLoader::getIndices() const noexcept
{
	return m_indices;
}


std::span<const TiledMapLoader::Object> TiledMapLoader::getObjects() const noexcept
{
	return m_objects;
}


std::string_view TiledMapLoader::getTileMask() const noexcept
{
	return m_tileMask;
}


ivec2s TiledMapLoader::getMapSize()  const noexcept
{
	return m_mapSize;
}


ivec2s TiledMapLoader::getTileSize() const noexcept
{
	return m_tileSize;
}


bool TiledMapLoader::loadLayers(const void* rootNode) noexcept
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

	for (auto layerNode = mapNode->first_node("layer");
			  layerNode != nullptr;
			  layerNode = layerNode->next_sibling("layer"))
	{
		auto attrName = layerNode->first_attribute("name");
		std::string name = attrName ? attrName->value() : std::string();

		auto dataNode = layerNode->first_node("data");

		if (!dataNode)
			continue;

		std::string csv(dataNode->value(), dataNode->value_size());
		std::string token;
		std::istringstream iss(csv);
		std::vector<int> tileIDs;

		const size_t tileCount = std::count_if(csv.begin(), csv.end(),
			[](const char c) { return c == ','; });

		tileIDs.reserve(tileCount + 1);

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

			if(strcmp(attrName->value(), "Structures") == 0)
				loadStructures(*currentTileset, tileIDs);
		}
	}

	return (!m_vertices.empty());
}


bool TiledMapLoader::loadObjects(const void* rootNode) noexcept
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

					for (auto attribute = propertyNode->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
					{
						if (strcmp(attribute->name(), "name") == 0) { property.name  = { attribute->value(), attribute->value_size() }; continue; }
						if (strcmp(attribute->name(), "type") == 0) { property.type  = { attribute->value(), attribute->value_size() }; continue; }
						if (strcmp(attribute->name(), "value") == 0)  property.value = { attribute->value(), attribute->value_size() };
					}
				}
			}
		}
	}

	return ( ! m_objects.empty() );
}


void TiledMapLoader::loadLandscape(const Tileset& tileset, std::span<const int32_t> tileIds) noexcept
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

	size_t tileIndex = 0;

	for (int32_t y = 0; y < mapHeight; ++y)
	{
		for (int32_t x = 0; x < mapWidth; ++x)
		{
			const int32_t tileID = tileIds[y * mapWidth + x];
			const int32_t tileNum = tileID - tileset.firstGID;

			m_tileMask[tileIndex] = convert_tile_num_to_char(tileID);
			++tileIndex;

			const int32_t tileY = (tileNum >= tileset.columns) ? tileNum / tileset.columns : 0;
			const int32_t tileX = tileNum % tileset.columns;

			const int32_t positionX = tileX * tileWidth;
			const int32_t positionY = tileY * tileHeight;
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


void TiledMapLoader::loadStructures(const Tileset& tileset, std::span<const int> tileIds) noexcept
{
	auto get_structure_type = [](int tileNum) -> StructureInfo::Type
	{
		switch (tileNum) // start num of tile in grid
		{
			case 111:
			case 112:
			case 113:
			case 114:
			case 115:
			case 116:
			case 117:
			case 118:
			case 119:
			case 120:
			case 121:
			case 122: return StructureInfo::Type::WALL;
			case 124: return StructureInfo::Type::REFINERY;
			case 127: return StructureInfo::Type::CONSTRUCTION_YARD;
			case 129: return StructureInfo::Type::WIND_TRAP;
			case 131: return StructureInfo::Type::OUTPOST;
			case 133: return StructureInfo::Type::SILO;
			case 135: return StructureInfo::Type::VEHICLE;
			case 159: return StructureInfo::Type::BARRACKS;
			case 161: return StructureInfo::Type::PALACE;
			case 164: return StructureInfo::Type::HIGH_TECH;
			case 166: return StructureInfo::Type::REPAIR;
			case 191: return StructureInfo::Type::SLAB_1x1;
			case 207: return StructureInfo::Type::STARPORT;
			case 261: return StructureInfo::Type::TURRET;
			case 269: return StructureInfo::Type::ROCKET_TURRET;
		
			default: return StructureInfo::Type::INVALID;
		}
	};


	auto get_structure_name = [](StructureInfo::Type type) -> std::string_view
	{
		switch (type)
		{
			case StructureInfo::Type::WALL:              return "Wall";
			case StructureInfo::Type::REFINERY:          return "Refinery";
			case StructureInfo::Type::CONSTRUCTION_YARD: return "ConstructYard";
			case StructureInfo::Type::WIND_TRAP:         return "WindTrap";
			case StructureInfo::Type::OUTPOST:           return "Outpost";
			case StructureInfo::Type::SILO:              return "Silo";
			case StructureInfo::Type::VEHICLE:           return "Vehicle";
			case StructureInfo::Type::BARRACKS:          return "Barracks";
			case StructureInfo::Type::PALACE:            return "Palace";
			case StructureInfo::Type::HIGH_TECH:         return "HighTech";
			case StructureInfo::Type::REPAIR:            return "Repair";
			case StructureInfo::Type::SLAB_1x1:          return "Slab_1x1";
			case StructureInfo::Type::STARPORT:          return "Starport";
			case StructureInfo::Type::TURRET:            return "Turret";
			case StructureInfo::Type::ROCKET_TURRET:     return "RocketTurret";
		
			default:
				return {};
		}
	};


	auto get_structure_tile_bounds = [](StructureInfo::Type type) -> ivec2s
	{
		switch (type)
		{
            case StructureInfo::Type::SLAB_1x1:          return { 1, 1 };
			case StructureInfo::Type::PALACE:            return { 3, 3 };
			case StructureInfo::Type::VEHICLE:           return { 3, 2 };
			case StructureInfo::Type::HIGH_TECH:         return { 2, 2 };
            case StructureInfo::Type::CONSTRUCTION_YARD: return { 2, 2 };
			case StructureInfo::Type::WIND_TRAP:         return { 2, 2 };
            case StructureInfo::Type::BARRACKS:          return { 2, 2 };
			case StructureInfo::Type::STARPORT:          return { 3, 3 };
			case StructureInfo::Type::REFINERY:          return { 3, 2 };
			case StructureInfo::Type::REPAIR:            return { 3, 2 };
            case StructureInfo::Type::WALL:              return { 1, 1 };
            case StructureInfo::Type::TURRET:            return { 1, 1 };
            case StructureInfo::Type::ROCKET_TURRET:     return { 1, 1 };
            case StructureInfo::Type::SILO:              return { 2, 2 };
            case StructureInfo::Type::OUTPOST:           return { 2, 2 };

            default: return { 0, 0 };
		}
	};

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
			
			if(const auto type = get_structure_type(tileID); type != StructureInfo::Type::INVALID)
			{
				auto& object = m_objects.emplace_back();

				object.name = get_structure_name(type); 
				object.type = "Structure";
				object.coords = { x, y };
				object.size = get_structure_tile_bounds(type);
			}
		}
	}
}


char convert_tile_num_to_char(int32_t index) noexcept
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
//  Rocky soil
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
//  Sandy soil
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