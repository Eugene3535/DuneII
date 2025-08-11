#ifndef MISSION_HPP
#define MISSION_HPP

#include <functional>

#include <SFML/Graphics/Sprite.hpp>
#include <entt/entity/registry.hpp>

#include "scenes/mission/tilemap/TileMap.hpp"
#include "scenes/Scene.hpp"
#include "ui/Cursor.hpp"

class Mission:
    public Scene
{
public:
    Mission(class DuneII* game) noexcept;
    ~Mission();

    bool load(const std::string& info) noexcept override;
    void update(sf::Time dt)           noexcept override;

private:
    bool loadAnimations() noexcept;
    void createSystems() noexcept;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<std::function<void(sf::Time)>> m_systems;

    sf::IntRect              m_viewport;
    TileMap                  m_tilemap;
    entt::registry           m_registry;
    std::vector<sf::Sprite*> m_sprites;
    Cursor                   m_cursor;
};

#endif // !MISSION_HPP