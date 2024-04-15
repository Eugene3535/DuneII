#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <string>
#include <vector>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "rapidxml.hpp"

class TileMap
{
private:
	struct TilesetData
	{
		const sf::Texture* texture = nullptr;
		std::int32_t columns   = 0;
		std::int32_t rows      = 0;
		std::int32_t tileCount = 0;
		std::int32_t firstGID  = 1;
	};

public:
	struct Layer // One tile layer per tileset
	{
		std::string        name;
		sf::VertexBuffer   vertexBuffer;
		const sf::Texture* texture = nullptr;
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
	void reset() noexcept;

private:
	bool loadLayers(const rapidxml::xml_node<>* map_node)  noexcept;
	bool loadObjects(const rapidxml::xml_node<>* map_node) noexcept;

private:
	void parseTilesets(const rapidxml::xml_node<char>* map_node, std::vector<TilesetData>& tilesets) noexcept;
	void parseCSVdata(const rapidxml::xml_node<char>* map_node, std::vector<std::int32_t>& parsed_layer) noexcept;
	void parseBuildings(const rapidxml::xml_node<char>* layer_node) noexcept;

public:
	std::vector<Layer>      layers;
	std::vector<Object>     objects;
	std::vector<sf::Vertex> staticBuildings;
	std::vector<sf::Vertex> animatedBuildings;
	std::string             title;
	sf::Vector2i            mapSize;
	sf::Vector2i            tileSize;
};

#endif // !TILE_MAP_HPP