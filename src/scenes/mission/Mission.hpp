#ifndef MISSION_HPP
#define MISSION_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <entt/entity/registry.hpp>

#include "scenes/mission/tilemap/TileMap.hpp"
#include "scenes/base/Scene.hpp"
#include "ecs/SystemManager.hpp"
#include "ui/Cursor.hpp"

class Mission:
    public Scene
{
public:
    Mission(struct Game& game) noexcept;
    ~Mission();

    bool load(const std::string& info) noexcept override;
    void update(sf::Time dt)           noexcept override;

private:
    bool loadAnimations() noexcept;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    TileMap                  m_tilemap;
    entt::registry           m_registry;
    SystemManager            m_systems;
    std::vector<sf::Sprite*> m_sprites;
    Cursor                   m_cursor;
};

#endif // !MISSION_HPP