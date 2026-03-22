#ifndef TILED_MAP_LOADER_HPP
#define TILED_MAP_LOADER_HPP

#include <string>
#include <vector>
#include <span>
#include <filesystem>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Vertex.hpp>


class TiledMapLoader
{
public:
	struct Object
	{
		struct Property
		{
			std::string name;
			std::string type;
			std::string value;
		};

		std::string           name;
		std::string           type;
		sf::Vector2i          coords; // presented in tiles
		sf::Vector2i          size;   // measured in tiles
		std::vector<Property> properties;
	};

	TiledMapLoader() noexcept;

	bool loadFromFile(const std::filesystem::path& filePath) noexcept;
	bool loadFromSQLiteDb(const std::filesystem::path& filePath) noexcept;
	void reset() noexcept;

	std::string_view getTitle() const noexcept;

	std::span<const sf::Vertex> getVertices() const noexcept;
	std::span<const Object>     getObjects()  const noexcept;
	std::string_view            getTileMask() const noexcept;

	sf::Vector2i getMapSize()  const noexcept;
	sf::Vector2i getTileSize() const noexcept;
	
private:
    bool loadLayers(const void* rootNode) noexcept;
    bool loadObjects(const void* rootNode) noexcept;
	void loadLandscape(const struct Tileset& tileset, std::span<const int32_t> tileIds) noexcept;
	void loadStructures(const struct Tileset& tileset, std::span<const int32_t> tileIds) noexcept;

	std::string m_title;

	std::vector<sf::Vertex> m_vertices;
	std::vector<Object>     m_objects;
	std::string             m_tileMask;
	
	sf::Vector2i m_mapSize;
	sf::Vector2i m_tileSize;
};

#endif // !TILED_MAP_LOADER_HPP