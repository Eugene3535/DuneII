#pragma once

#include "files/TileMap.hpp"
#include "scenes/mission/level/Level.hpp"
#include "scenes/mission/HUD/HeadUpDisplay.hpp"
#include "scenes/mission/HUD/construction/ConstructionMenu.hpp"
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

    struct System
    {
        void (*execute)(Mission*, float) = nullptr;
        uint32_t frequency = 1;
        bool isEnabled = true;
    };
    std::vector<System> m_systems;

    TileMap        m_tilemap;
    Level          m_level;
    entt::registry m_registry;

    ConstructionMenu m_menu;
    HeadUpDisplay    m_hud;
};
