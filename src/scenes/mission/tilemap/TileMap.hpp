#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include <SFML/Graphics/VertexBuffer.hpp>

#include "RapidXML/rapidxml.hpp"

#include "scenes/mission/buildings/Building.hpp"

class TileMap:
	public sf::Drawable
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
	TileMap() noexcept;

	bool loadFromFile(const std::filesystem::path& file_path) noexcept;
	void unload() noexcept;

	std::vector<Building*>     getAllBuildings()          noexcept;
	const std::vector<Object>& getObjects()         const noexcept;
	std::string_view           getTileMask()        const noexcept;
	const sf::Vector2i&        getMapSizeInTiles()  const noexcept;
	const sf::Vector2i&        getMapSizeInPixels() const noexcept;
	const sf::Vector2i&        getTileSize()        const noexcept;
	
private:
	bool loadLayers(const rapidxml::xml_node<>* map_node)  noexcept;
	bool loadObjects(const rapidxml::xml_node<>* map_node) noexcept;

private:
	void parseTilesets(const rapidxml::xml_node<>* map_node, std::vector<Tileset>& tilesets) noexcept;
	void parseLandscape(const Tileset& tileset, const std::vector<int>& parsed_layer)        noexcept;
	void parseBuildings(const Tileset& tileset, const std::vector<int>& parsed_layer)        noexcept;

private:
	char convertTileNumToChar(int32_t index) const noexcept;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::VertexBuffer   m_vertices;
	const sf::Texture* m_texture;

private:
    std::unordered_map<int32_t, Building> m_buildings;
	std::vector<Object>                   m_objects;
	std::vector<BuildingData>             m_buildingsOnLoad;
	std::string                           m_tileMask;
	
private:
	std::string  m_title;
	sf::Vector2i m_mapSizeInTiles;
	sf::Vector2i m_mapSizeInPixels;
	sf::Vector2i m_tileSize;
};

#endif // !TILEMAP_HPP