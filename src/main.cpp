#include "game/Game.hpp"

int main()
{
    if(Game game(nullptr); game.load())
        return game.run(); 

    return -1;
}