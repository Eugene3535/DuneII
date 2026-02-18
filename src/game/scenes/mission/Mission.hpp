#ifndef MISSION_HPP
#define MISSION_HPP

#include "game/scenes/mission/loader/TiledMapLoader.hpp"
#include "game/scenes/mission/tilemap/Tilemap.hpp"
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
    void createSystems() noexcept;

    Transform2D    m_transform;
    TiledMapLoader m_mapLoader;
    entt::registry m_registry;
    Tilemap        m_tilemap;
    HeadUpDisplay  m_hud;

    std::vector<void(*)(Mission*, float)> m_systems;
};

#endif // !MISSION_HPP