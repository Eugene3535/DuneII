#ifndef TILED_MAP_LOADER_HPP
#define TILED_MAP_LOADER_HPP

#include <string>
#include <vector>
#include <span>
#include <filesystem>

#include <cglm/struct/ivec2.h>
#include <cglm/struct/ivec4.h>


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
		ivec2s                coords; // presented in tiles
		ivec2s                size;   // measured in tiles
		std::vector<Property> properties;
	};

	TiledMapLoader() noexcept;

	bool loadFromFile(const std::filesystem::path& filePath) noexcept;
	bool loadFromSQLiteDb(const std::filesystem::path& filePath) noexcept;
	void reset() noexcept;

	std::string_view getTitle() const noexcept;

	std::span<const vec4s>    getVertices() const noexcept;
	std::span<const uint32_t> getIndices()  const noexcept;
	std::span<const Object>   getObjects()  const noexcept;
	std::string_view          getTileMask() const noexcept;

	ivec2s getMapSize()  const noexcept;
	ivec2s getTileSize() const noexcept;
	
private:
    bool loadLayers(const void* rootNode) noexcept;
    bool loadObjects(const void* rootNode) noexcept;
	void loadLandscape(const struct Tileset& tileset, std::span<const int32_t> tileIds) noexcept;
	void loadStructures(const struct Tileset& tileset, std::span<const int32_t> tileIds) noexcept;

	std::string m_title;

	std::vector<vec4s>    m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<Object>   m_objects;
	std::string           m_tileMask;
	
	ivec2s m_mapSize;
	ivec2s m_tileSize;
};

#endif // !TILED_MAP_LOADER_HPP