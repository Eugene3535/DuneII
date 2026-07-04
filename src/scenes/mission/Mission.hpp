#pragma once

#include "files/TiledMapLoader.hpp"
#include "scenes/mission/tilemap/TileMap.hpp"
#include "scenes/mission/HUD/HeadUpDisplay.hpp"
#include "scenes/mission/HUD/construction/ConstructionMenu.hpp"
#include "scenes/Scene.hpp"


class Mission:
    public Scene
{
public:
    explicit Mission(class Game* game) noexcept;
    ~Mission();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw(const mat4s& projection) noexcept override;
    void resize(int width, int height) noexcept override;

private:
    void createSystems() noexcept;

    TiledMapLoader   m_mapLoader;
    TileMap          m_tilemap;
    entt::registry   m_registry;

    ConstructionMenu m_menu;
    HeadUpDisplay    m_hud;

    std::vector<void(*)(Mission*, float)> m_systems;
};
