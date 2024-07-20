#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <string>
#include <vector>
#include <filesystem>

#include "rapidxml.hpp"

#include "common/Defines.hpp"
#include "common/ObjectPool.hpp"
#include "scenes/mission/buildings/Building.hpp"

class TileMap
{
private:
	struct Tileset
	{
		const sf::Texture* texture { nullptr };
		int32_t columns   { 0 };
		int32_t tileCount { 0 };
		int32_t firstGID  { 1 };
	};

public:
	struct Landscape 
	{
		sf::VertexBuffer vertices;
		const sf::Texture* texture { nullptr };
	};

public:
	struct Object
	{
		struct Property
		{
			std::string name;
			std::string type;
			std::string value;
		};

		std::vector<Property> properties;
		std::string           name;
		std::string           type;
		sf::Vector2i          position;
		sf::Vector2i          size;
	};

public:
	bool loadFromFile(const std::filesystem::path& file_path) noexcept;

	int32_t costOf(Building::Type type) const noexcept;
	Building* placeBuilding(Building::Type type, int32_t cellX, int32_t cellY) noexcept;
	void eraseBuilding(const Building* building) noexcept;
	std::vector<Building*> getAllBuildings() noexcept;
	void unload() noexcept;

private:
	bool loadLayers(const rapidxml::xml_node<>* map_node)  noexcept;
	bool loadObjects(const rapidxml::xml_node<>* map_node) noexcept;

private:
	void parseTilesets(const rapidxml::xml_node<char>* map_node, std::vector<Tileset>& tilesets)   noexcept;
	void parseLandscape(const Tileset& tileset, const std::vector<int>& parsed_layer)     noexcept;
	void parseBuildings(const Tileset& tileset, const std::vector<int>& parsed_layer)     noexcept;

private:
	char        convertTileNumToChar(int32_t index) const noexcept;
    sf::IntRect getTexCoordsOf(Building::Type type) const noexcept;
	sf::IntRect getBoundsOf(Building::Type type, int32_t coordX, int32_t coordY) const noexcept;
	int32_t     getHitPointsOf(Building::Type type) const noexcept;

private:
    ObjectPool<Building, ALL_BUILDING_LIMIT_ON_MAP> m_buildings;

public:
	Landscape landscape;
	std::vector<Object>     objects;
	std::string             tileMask;
	std::vector<char*> collisionMask;
	
public:
	std::string  title;
	sf::Vector2i mapSizeInTiles;
	sf::Vector2i mapSizeInPixels;
	sf::Vector2i tileSize;
};

#endif // !TILE_MAP_HPP