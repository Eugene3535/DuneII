#ifndef BUILDING_MANAGER_HPP
#define BUILDING_MANAGER_HPP

#include "utils/Defines.hpp"
#include "utils/ObjectPool.hpp"
#include "scenes/mission/buildings/Building.hpp"

class BuildingManager
{
public:
    BuildingManager() noexcept;
    ~BuildingManager();

    bool load(class TileMap* tilemap) noexcept;

    Building* construct(Building::Type type, int32_t cellX, int32_t cellY) noexcept;
    void      destroy(const Building* target) noexcept;
    int32_t   costOf(Building::Type type) const noexcept;

private:
    sf::IntRect getTexCoords(Building::Type type) const noexcept;

private:
    ObjectPool<Building, BUILDING_MAX_COUNT> m_buildings;
};

#endif // !BUILDING_MANAGER_HPP