#include "game/scenes/mission/tilemap/Tilemap.hpp"
#include "game/scenes/mission/base/Base.hpp"


Base::Base(TileMap& tilemap) noexcept:
    m_tilemap(tilemap),
    m_spice(0),
    m_energy(0)
{

}


Base::~Base() = default;


void Base::removeStructure(const entt::entity entity) noexcept
{

}