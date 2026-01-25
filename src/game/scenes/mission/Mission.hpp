#ifndef MISSION_HPP
#define MISSION_HPP

#include "graphics/sprites/SpriteManager.hpp"
#include "game/scenes/mission/tilemap/TileMap.hpp"
#include "game/scenes/mission/builder/Builder.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"
#include "game/scenes/mission/menu/ConstructionMenu.hpp"
#include "game/scenes/Scene.hpp"


class Mission:
    public Scene
{
public:
    explicit Mission(class DuneII* game) noexcept;
    ~Mission();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw()                        noexcept override;

private:
    bool initLandscape() noexcept;
    bool initHUD() noexcept;
    void createSystems() noexcept;

    Transform2D      m_transform;
    TileMap          m_tilemap;
    SpriteManager    m_sprites;
    entt::registry   m_registry;
    Builder          m_builder;
    std::string      m_tileMask;
    HeadUpDisplay    m_hud;
    ConstructionMenu m_menu;
    
//  Resource section
    struct
    {
        uint32_t texture;
        uint32_t vao;
        uint32_t vbo[2];
        uint32_t count; // indices
        uint32_t program;
    } m_landscape;

    struct
    {
        uint32_t texture;
        uint32_t vao;
    } m_buildings;

    struct
    {
        uint32_t texture;
        uint32_t program;
        float clickTimer;
    } m_ui;

    struct
    {
        uint32_t frameProgram;
        uint32_t previewProgram;
    } m_construction;

    std::vector<void(*)(Mission*, float)> m_systems;
};

#endif // !MISSION_HPP