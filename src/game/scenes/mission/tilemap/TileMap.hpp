#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <entt/entity/registry.hpp>

#include "game/scenes/mission/common/Structures.hpp"


class Tilemap final:
    public sf::Drawable
{
public:
    Tilemap(class DuneII* game, entt::registry& registry) noexcept;
    ~Tilemap();

    bool createFromLoader(const class TiledMapLoader& loader) noexcept;
    bool putStructure(const HouseType owner, const StructureInfo::Type type, const sf::Vector2i point) noexcept;

    entt::entity    getEntityUnderCursor(const sf::Vector2i point) const noexcept;
    entt::registry& getRegistry()                                  const noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateWall(int32_t origin, int32_t level) noexcept;

    DuneII*         m_game;
    entt::registry& m_registry;

    std::string               m_tileMask;
    std::vector<entt::entity> m_structureMask;

    sf::VertexBuffer   m_vertexBuffer;
	const sf::Texture* m_landscapeTexture;
    const sf::Texture* m_structureTexture;

    sf::Vector2i m_mapSize;
    sf::Vector2i m_tileSize;
};

#endif // !TILEMAP_HPP