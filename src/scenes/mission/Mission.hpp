#ifndef MISSION_HPP
#define MISSION_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <entt/entity/registry.hpp>

#include "scenes/mission/tilemap/TileMap.hpp"
#include "scenes/Scene.hpp"
#include "ecs/SystemManager.hpp"

class Mission:
    public Scene
{
public:
    Mission(struct Game& game) noexcept;
    ~Mission();

    bool load(const std::string& info) noexcept override;
    void update(sf::Time dt)           noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    TileMap                  m_tilemap;
    entt::registry           m_registry;
    SystemManager            m_systems;
    std::vector<sf::Sprite*> m_sprites;

    sf::Vector2i m_viewPosition;
    sf::Vector2i m_previousCursorPosition;
};

#endif // !MISSION_HPP