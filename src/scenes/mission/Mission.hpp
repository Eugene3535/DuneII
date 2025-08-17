#ifndef MISSION_HPP
#define MISSION_HPP

#include <SFML/Graphics/Drawable.hpp>
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
    void update(const sf::Time dt)     noexcept override;

    sf::Vector2i resize(const sf::Vector2u& size) noexcept override;

private:
    bool loadAnimations() noexcept;
    void createSystems() noexcept;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<void(*)(Mission*, sf::Time)> m_systems;

    sf::IntRect                m_viewport;
    sf::Vector2i               m_viewPosition;
    TileMap                    m_tilemap;
    entt::registry             m_registry;
    std::vector<sf::Drawable*> m_sprites;
    Cursor                     m_cursor;
};

#endif // !MISSION_HPP