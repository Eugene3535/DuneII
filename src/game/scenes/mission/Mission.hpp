#ifndef MISSION_HPP
#define MISSION_HPP

#include <memory>

#include "graphics/Meshes.hpp"
#include "game/scenes/mission/tilemap/TileMap.hpp"
#include "game/scenes/Scene.hpp"


class Mission:
    public Scene
{
public:
    explicit Mission(class DuneII* game) noexcept;

    bool load(std::string_view info)         noexcept override;
    void update(float dt)                    noexcept override;
    void draw()                              noexcept override;
    void resize(int width, int height)       noexcept override;
    void press(int key)                      noexcept override;
    void click(int button)                   noexcept override;
    void setCursorPosition(float x, float y) noexcept override;

private:
    void createSystems() noexcept;

    std::unique_ptr<TileMap> m_tilemap;
    Plane m_landscape;
    Transform2D m_landscapeTransform;
    uint32_t m_shaderProgram;

    std::vector<void(*)(Mission*, float)> m_systems;
};

#endif // !MISSION_HPP