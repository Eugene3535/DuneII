#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include <SFML/Graphics/VertexBuffer.hpp>
#include "RapidXML/rapidxml.hpp"
#include <entt/entity/registry.hpp>

#include "common/Enums.hpp"

class TileMap:
	public sf::Drawable
{
	struct Tileset
	{
		const sf::Texture* texture = nullptr ;
		int32_t columns   = 0;
		int32_t tileCount = 0;
		int32_t firstGID  = 1;
	};

	enum WallCellType : uint32_t
    {
        DOT = 1 << 1,
        LEFT_RIGHT = 1 << 2,
        BOTTOM_TOP = 1 << 3,
        TOP_RIGHT = 1 << 4,
        RIGHT_BOTTOM = 1 << 5,
        BOTTOM_LEFT = 1 << 6,
        LEFT_TOP = 1 << 7,
        TOP_RIGHT_BOTTOM = 1 << 8,
        RIGHT_BOTTOM_LEFT = 1 << 9,
        BOTTOM_LEFT_TOP = 1 << 10,
        LEFT_TOP_RIGHT = 1 << 11,
        CROSS = 1 << 12
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
	TileMap(entt::registry& registry, class AnimationManager& animationManager) noexcept;

	bool loadFromFile(const std::filesystem::path& file_path)                noexcept;
	void unload()                                                            noexcept;
	bool putStructureOnMap(StructureType type, int32_t cellX, int32_t cellY) noexcept;
	void removeStructureFromMap(int32_t structureId)                         noexcept;

	const std::vector<Object>& getObjects()         const noexcept;
	std::string_view           getTileMask()        const noexcept;
	const sf::Vector2i&        getMapSizeInTiles()  const noexcept;
	const sf::Vector2i&        getMapSizeInPixels() const noexcept;
	const sf::Vector2i&        getTileSize()        const noexcept;
	
private:
	bool         loadLayers(const rapidxml::xml_node<>* map_node)                                    noexcept;
	bool         loadObjects(const rapidxml::xml_node<>* map_node)                                   noexcept;
	void         loadTilesets(const rapidxml::xml_node<>* map_node, std::vector<Tileset>& tilesets)  noexcept;
	void         loadLandscape(const Tileset& tileset, const std::vector<int>& parsed_layer)         noexcept;
	void         loadStructures(const Tileset& tileset, const std::vector<int>& parsed_layer)        noexcept;
	char         convertTileNumToChar(int32_t index)                                           const noexcept;
	void         draw(sf::RenderTarget& target, sf::RenderStates states)                       const override;
	void         updateWall(int32_t origin, int32_t level)                                           noexcept;
    WallCellType getWallType(bool left, bool top, bool right, bool bottom)                           noexcept;
    sf::IntRect  getTexCoordsOf(WallCellType type)                                                   noexcept;
    sf::IntRect  getTexCoordsOf(StructureType type)                                            const noexcept;
	sf::IntRect  getBoundsOf(StructureType type, int32_t coordX, int32_t coordY)               const noexcept;
	int32_t      getHitPointsOf(StructureType type)                                            const noexcept;

private:
	std::unordered_map<int32_t, entt::entity> m_structuresById;
	class AnimationManager&   m_animationManager;
	entt::registry&           m_registry;
	sf::VertexBuffer          m_vertices;
	const sf::Texture*        m_texture;
	std::vector<Object>       m_objects;
	std::string               m_tileMask;
	std::string               m_title;
	sf::Vector2i              m_mapSizeInTiles;
	sf::Vector2i              m_mapSizeInPixels;
	sf::Vector2i              m_tileSize;
};

#endif // !TILEMAP_HPP