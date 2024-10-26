#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <vector>
#include <memory>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "assets/AssetManager.hpp"
#include "animation/AnimationManager.hpp"
#include "effects/ScreenBlackoutEffect.hpp"

struct Game
{
public:
    enum GameScene: uint32_t
    {
        NONE,
        MAIN_MENU,
        MISSION
    };

    sf::RenderWindow window;
    sf::View         viewport;
    sf::FloatRect    visible_area;
    sf::Clock        clock;

    AnimationManager animationManager;

    GameScene next_scene { NONE };

    bool sceneNeedToBeChanged { false };
};

#endif // !GAME_HPP