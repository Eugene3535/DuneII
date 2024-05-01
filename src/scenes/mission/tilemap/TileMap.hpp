#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <string>
#include <vector>
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
		std::int32_t columns   { 0 };
		std::int32_t tileCount { 0 };
		std::int32_t firstGID  { 1 };
	};

public:
	template<class T>
	class Layer 
	{
	public:
		T vertices;
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
	void reset() noexcept;

private:
	bool loadLayers(const rapidxml::xml_node<>* map_node)  noexcept;
	bool loadObjects(const rapidxml::xml_node<>* map_node) noexcept;

private:
	void parseTilesets(const rapidxml::xml_node<char>* map_node, std::vector<Tileset>& tilesets) noexcept;
	void parseLandscape(const Tileset& tileset, const std::vector<std::int32_t>& parsed_layer) noexcept;
	void parseBuildings(const Tileset& tileset, const std::vector<std::int32_t>& parsed_layer) noexcept;

private:
	char convertTileNumToChar(std::int32_t index) const noexcept;

public:
	Layer<sf::VertexBuffer>        landscape;
	// Layer<std::vector<sf::Vertex>> staticTiles;
	// Layer<std::vector<sf::Vertex>> animatedTiles;
	std::vector<Object>            objects;
	std::string                    tileMask;
	
public:
	std::string  title;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
};

#endif // !TILE_MAP_HPP