#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <vector>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "loaders/Assets.hpp"

#include "effects/ScreenBlackoutEffect.hpp"

struct Game
{
public:
    enum GameScene: std::uint32_t
    {
        NONE,
        MAIN_MENU,
        MISSION
    };

    sf::RenderWindow window;
    sf::View         viewport;
    sf::FloatRect    visible_area;
    sf::Clock        clock;

    GameScene next_scene { NONE };

    bool sceneNeedToBeChanged { false };
};

#endif