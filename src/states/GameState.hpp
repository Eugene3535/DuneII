#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <cstdint>

struct GameState
{
    enum GameScene: std::uint32_t
    {
        NONE,
        INTRO,
        MAIN_MENU,
        BATTLE
    };

    static GameScene next_scene;

    static bool A_SCENE_NEEDS_TO_BE_CHANGED;
};


#endif // !