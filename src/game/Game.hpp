#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "managers/AssetManager.hpp"
#include "managers/SceneManager.hpp"

#include "effects/ScreenBlackoutEffect.hpp"

class Game final
{
public:
    enum GameScene: std::uint32_t
    {
        NONE,
        MAIN_MENU,
        MISSION
    };

public:
    Game() noexcept;
    ~Game();

    void         setSpriteSize(sf::Sprite& sprite, std::int32_t width, std::int32_t height) noexcept;
    void         setSpriteSize(sf::Sprite& sprite, const sf::Vector2i& size) noexcept;
    sf::Vector2i getSpriteSize(const sf::Sprite& sprite) noexcept;

    std::int32_t run() noexcept;

public:
    std::unique_ptr<sf::RenderWindow> window;
    sf::View      viewport;
    sf::FloatRect visible_area;
    sf::Clock clock;

public:
    AssetManager assets;
    SceneManager scenes;

private:
    ScreenBlackoutEffect m_fade_effect;

public:
    GameScene next_scene;

public:
    bool a_scene_needs_to_be_changed;
    bool m_is_blackout_mode;
};

#endif