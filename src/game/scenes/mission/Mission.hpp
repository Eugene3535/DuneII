#ifndef MISSION_HPP
#define MISSION_HPP

#include "resources/gl_interfaces/shaders/ShaderProgram.hpp"
#include "game/scenes/mission/tilemap/TileMap.hpp"
#include "game/scenes/mission/builder/Builder.hpp"
#include "game/scenes/Scene.hpp"


class Mission:
    public Scene
{
public:
    explicit Mission(class DuneII* game) noexcept;
    ~Mission();

    bool load(std::string_view info)         noexcept override;
    void update(float dt)                    noexcept override;
    void draw()                              noexcept override;
    void resize(int width, int height)       noexcept override;

private:
    void createSystems() noexcept;

    Transform2D    m_transform;
    TileMap        m_tilemap;
    SpriteManager  m_sprites;
    entt::registry m_registry;
    Builder        m_builder;
    std::string    m_tileMask;
    
    struct
    {
        GLuint texture;
        GLuint vao;
        GLuint vbo[2];
        GLuint count; // indices
        ShaderProgram program;
    } m_landscape;

    struct
    {
        GLuint texture;
        GLuint vao;
    } m_buildings;
    
    std::vector<void(*)(Mission*, float)> m_systems;
};

#endif // !MISSION_HPP