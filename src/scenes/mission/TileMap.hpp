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
	bool loadFromFile(const std::filesystem::path& fPath) noexcept;

private:
	bool loadLayers(const rapidxml::xml_node<char>* mapNode)  noexcept;
	bool loadObjects(const rapidxml::xml_node<char>* mapNode) noexcept;

private:
	std::vector<TilesetData>  parseTilesets(const rapidxml::xml_node<char>* mapNode)  noexcept;
	std::vector<std::int32_t> parseCSVstring(const rapidxml::xml_node<char>* mapNode) noexcept;

public:
	std::vector<Layer>  m_layers;
	std::vector<Object> m_objects;
	std::string         m_collisionMask;
	std::string         m_title;
	sf::Vector2i        m_mapSize;
	sf::Vector2i        m_tileSize;
};

#endif // !TILE_MAP_HPP