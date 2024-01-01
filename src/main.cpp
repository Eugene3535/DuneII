#include "game/Game.hpp"

int main()
{
    if(Game game(nullptr); game.isLoaded())
        return game.run(); 

    return -1;
}