#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "rapidxml.hpp"

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
	public:
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
	Building* placeBuilding(int32_t x, int32_t y, Building::Type type) noexcept;
	void reset() noexcept;

private:
	bool loadLayers(const rapidxml::xml_node<>* map_node)  noexcept;
	bool loadObjects(const rapidxml::xml_node<>* map_node) noexcept;

private:
	void parseTilesets(const rapidxml::xml_node<char>* map_node, std::vector<Tileset>& tilesets)   noexcept;
	void parseLandscape(const Tileset& tileset, const std::vector<int32_t>& parsed_layer)     noexcept;
	void parseBuildings(const Tileset& tileset, const std::vector<int32_t>& parsed_layer)     noexcept;

private:
	int8_t convertTileNumToChar(int32_t index) const noexcept;

public:
	Landscape landscape;
	std::vector<std::unique_ptr<Building>> buildings;
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