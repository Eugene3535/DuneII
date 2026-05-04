#ifndef GAME_INFO_HPP
#define GAME_INFO_HPP

#include <cstdint>
#include <vector>

#include "game/scenes/mission/common/Structures.hpp"
#include "game/scenes/mission/HUD/previews/EntityPreview.hpp"


class GameInfo
{
public:
    GameInfo() noexcept;

    std::vector<EntityPreview::Icon> getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) const noexcept;

private:

};

#endif // !GAME_INFO_HPP