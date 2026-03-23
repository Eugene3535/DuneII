#ifndef MISSION_HPP
#define MISSION_HPP

#include "game/scenes/mission/loader/TiledMapLoader.hpp"
#include "game/scenes/mission/tilemap/Tilemap.hpp"
// #include "game/scenes/mission/HUD/HeadUpDisplay.hpp"
#include "game/scenes/Scene.hpp"


class Mission:
    public Scene
{
public:
    explicit Mission(class DuneII* game) noexcept;
    ~Mission();

    bool load(std::string_view data) noexcept override;
    void update(sf::Time dt)         noexcept override;
    void resize(sf::Vector2u size)   noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void createSystems() noexcept;

    TiledMapLoader m_mapLoader;
    entt::registry m_registry;
    Tilemap        m_tilemap;

    sf::IntRect  m_viewport;
    sf::Vector2i m_viewPosition;
    // HeadUpDisplay  m_hud;

    std::vector<void(*)(Mission*, sf::Time dt)> m_systems;
};

#endif // !MISSION_HPP