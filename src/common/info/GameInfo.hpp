#pragma once

#include <vector>

#include <cglm/struct/ivec2.h>

#include "scenes/mission/info/StructureInfo.hpp"


namespace GameInfo
{
    StructureInfo::Type getStructureType(int tileNum) noexcept;
    ivec2s getStructureSizeInTiles(StructureInfo::Type type) noexcept;
    char convertTileNumToChar(int32_t index) noexcept;

    std::vector<EntityIcon> getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) noexcept;
}