#ifndef MISSION_HPP
#define MISSION_HPP

#include "game/scenes/mission/tilemap/TileMap.hpp"
#include "game/scenes/mission/builder/Builder.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"
#include "game/scenes/Scene.hpp"


class Mission:
    public Scene
{
public:
    explicit Mission(class Engine* engine) noexcept;
    ~Mission();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw()                        noexcept override;
    void resize(int width, int height) noexcept override;

private:
    bool initLandscape() noexcept;
    bool initHUD() noexcept;
    void createSystems() noexcept;

    Transform2D      m_transform;
    TileMap          m_tilemap;
    entt::registry   m_registry;
    std::string      m_tileMask;
    Builder          m_builder;
    HeadUpDisplay    m_hud;
    
//  Resource section
    mesh::Landscape m_landscape;
    mesh::Buildings m_buildings;

    std::vector<void(*)(Mission*, float)> m_systems;
};

#endif // !MISSION_HPP