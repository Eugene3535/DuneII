#pragma once

#include "files/TileMap.hpp"
#include "scenes/mission/level/Level.hpp"
#include "scenes/mission/ui/HeadUpDisplay.hpp"
#include "scenes/mission/ui/construction/ConstructionMenu.hpp"
#include "scenes/Scene.hpp"


class Mission:
    public Scene
{
public:
    explicit Mission(struct Game* game) noexcept;
    ~Mission();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw(const mat4s& projection) noexcept override;
    void resize(int width, int height) noexcept override;

private:
    void createSystems() noexcept;

    TileMap m_tilemap;
    Level   m_level;

    ConstructionMenu m_menu;
    HeadUpDisplay    m_hud;
};
