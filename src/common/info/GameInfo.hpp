#pragma once

#include <cstdint>
#include <vector>

#include "scenes/mission/common/Structures.hpp"


class GameInfo
{
public:
    GameInfo() noexcept;

    std::vector<EntityIcon> getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) const noexcept;

private:

};