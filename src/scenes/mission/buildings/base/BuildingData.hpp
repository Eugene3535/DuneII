#ifndef BUILDING_DATA_HPP
#define BUILDING_DATA_HPP

#include <cstdint>

struct BuildingData
{
    char** tileMask = nullptr;
    std::int32_t startCoordX = 0;
    std::int32_t startCoordY = 0;
    std::int32_t horizontalTileCount = 0;
    std::int32_t verticalTileCount = 0;
    std::int32_t mapWidth = 0;
    std::int32_t mapHeight = 0;
    
    bool hasAnAnimatedDynastyFlag = false;
    bool hasAnAnimatedLandingPad = false;
};


#endif // !BUILDING_DATA_HPP