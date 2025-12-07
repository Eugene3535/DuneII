#ifndef MISSION_HPP
#define MISSION_HPP

#include "resources/gl_interfaces/shaders/ShaderProgram.hpp"
#include "game/scenes/mission/tilemap/TileMap.hpp"
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
    void press(int key)                      noexcept override;
    void click(int button)                   noexcept override;
    void setCursorPosition(float x, float y) noexcept override;

private:
    void createSystems() noexcept;

    struct
    {
        GLuint textures[1];
        GLuint vbo[2];
        GLuint vao[1];
    } m_glHandles;

    TileMap m_tilemap;
    struct
    {
        GLuint texture;
        GLuint vao;
        GLuint count; // indices
        ShaderProgram shaderProgram;
        Transform2D transform;
    } m_landscape;

    std::vector<void(*)(Mission*, float)> m_systems;
};

#endif // !MISSION_HPP