#ifndef MISSION_HPP
#define MISSION_HPP

#include "graphics/sprites/SpriteManager.hpp"
#include "game/scenes/mission/tilemap/TileMap.hpp"
#include "game/scenes/mission/builder/Builder.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"
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
    void resize(int width, int height) noexcept override;

private:
    void createSystems() noexcept;

    Transform2D    m_transform;
    TileMap        m_tilemap;
    SpriteManager  m_sprites;
    entt::registry m_registry;
    Builder        m_builder;
    std::string    m_tileMask;
    HeadUpDisplay  m_hud;
    
//  Resource section
    struct
    {
        GLuint texture;
        GLuint vao;
        GLuint vbo[2];
        GLuint count; // indices
        GLuint m_program;
    } m_landscape;

    struct
    {
        GLuint texture;
        GLuint vao;
    } m_buildings;

    struct
    {
        GLuint texture;
    } m_ui;

    std::vector<void(*)(Mission*, float)> m_systems;
};

#endif // !MISSION_HPP