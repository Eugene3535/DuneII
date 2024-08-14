#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "rapidxml.hpp"

#include "scenes/mission/buildings/Building.hpp"

class TileMap
{
	friend class Builder;

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
		sf::IntRect           bounds;
	};

public:
	struct BuildingData 
	{
		Building::Type type;
		int32_t cellX;
		int32_t cellY;
	};

public:
	bool loadFromFile(const std::filesystem::path& file_path) noexcept;

	std::vector<Building*> getAllBuildings() noexcept;
	void                   unload() noexcept;

	const std::vector<Object>& getObjects() const noexcept;

private:
	bool loadLayers(const rapidxml::xml_node<>* map_node)  noexcept;
	bool loadObjects(const rapidxml::xml_node<>* map_node) noexcept;

private:
	void parseTilesets(const rapidxml::xml_node<>* map_node, std::vector<Tileset>& tilesets)   noexcept;
	void parseLandscape(const Tileset& tileset, const std::vector<int>& parsed_layer)     noexcept;
	void parseBuildings(const Tileset& tileset, const std::vector<int>& parsed_layer)     noexcept;

private:
	char convertTileNumToChar(int32_t index) const noexcept;

private:
    std::unordered_map<int32_t, Building> m_buildings;
	std::vector<Object> m_objects;
	std::vector<BuildingData> m_buildingsOnLoad;

public:
	Landscape landscape;
	
	std::string             tileMask;
	std::vector<char*> collisionMask;
	
public:
	std::string  title;
	sf::Vector2i mapSizeInTiles;
	sf::Vector2i mapSizeInPixels;
	sf::Vector2i tileSize;
};

#endif // !TILEMAP_HPP