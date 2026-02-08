#ifndef GAME_INFO_HPP
#define GAME_INFO_HPP

#include <cstdint>
#include <vector>

#include "game/scenes/mission/common/Structures.hpp"


class GameInfo
{
public:
    GameInfo() noexcept;

    std::vector<PreviewType> getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) const noexcept;

private:

};

#endif // !GAME_INFO_HPP