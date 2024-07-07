
#include "scenes/mission/tilemap/TileMap.hpp"
#include "managers/buildings/BuildingManager.hpp"

BuildingManager::BuildingManager() noexcept
{

}

BuildingManager::~BuildingManager()
{
}

bool BuildingManager::load(TileMap* tilemap) noexcept
{
    return false;
}

Building* BuildingManager::construct(Building::Type type, int32_t cellX, int32_t cellY) noexcept
{
    if(auto building = m_buildings.findUnusedObject(); building != nullptr)
    {

    }

    return nullptr;
}

void BuildingManager::destroy(const Building* target) noexcept
{
    m_buildings.returnObjectBack(target);
}

int32_t BuildingManager::costOf(Building::Type type) const noexcept
{
    switch (type)
    {
        case Building::CONCRETE_SLAB:      return 5;
        case Building::CONSTRUCTION_YARD:  return 900;
        case Building::SPICE_SILO:         return 300;
        case Building::STARPORT:           return 500;
        case Building::WIND_TRAP:          return 300;
        case Building::SPICE_REFINERY:     return 300;
        case Building::RADAR_OUTPOST:      return 400;
        case Building::REPAIR_FACILITY:    return 700;
        case Building::PALACE:             return 999;
        case Building::HIGH_TECH_FACILITY: return 500;
        case Building::BARRACKS:           return 300;
        case Building::VEHICLE_FACTORY:    return 400;
        case Building::WALL:               return 50;
        case Building::TURRET:             return 125;
        case Building::ROCKET_TURRET:      return 250;    

        default: return 0;
    }
}

sf::IntRect BuildingManager::getTexCoords(Building::Type type) const noexcept
{
    switch (type)
    {
        case Building::CONCRETE_SLAB:      return { 0,   160, 32, 32 };
        case Building::CONSTRUCTION_YARD:  return { 0,   32,  64, 64 };
        case Building::SPICE_SILO:         return { 192, 32,  64, 64 };
        case Building::STARPORT:           return { 0,   192, 96, 96 };
        case Building::WIND_TRAP:          return { 64,  32,  64, 64 };
        case Building::SPICE_REFINERY:     return { 416, 0,   96, 64 };
        case Building::RADAR_OUTPOST:      return { 128, 32,  64, 64 };
        case Building::REPAIR_FACILITY:    return { 224, 96,  96, 64 };
        case Building::PALACE:             return { 64,  96,  96, 96 };
        case Building::HIGH_TECH_FACILITY: return { 160, 96,  64, 64 };
        case Building::BARRACKS:           return { 0,   96,  64, 64 };
        case Building::VEHICLE_FACTORY:    return { 256, 32,  96, 64 };
        case Building::WALL:               return { 0,   0,   32, 32 };
        case Building::TURRET:             return { 192, 288, 32, 32 };
        case Building::ROCKET_TURRET:      return { 448, 288, 32, 32 };

        default: return sf::IntRect();
    }
}