#ifndef BASE_HPP
#define BASE_HPP

#include <entt/entity/fwd.hpp>

#include "common/Enums.hpp"


class Base
{
public:
    Base(class TileMap& tilemap) noexcept;
    ~Base();

private:
    void removeStructure(const entt::entity entity) noexcept;

    class TileMap& m_tilemap;

    HouseType m_house;

    int32_t m_spice;
    int32_t m_energy;
};

#endif // !BASE_HPP