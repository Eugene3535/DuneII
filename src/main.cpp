#include "game/Game.hpp"

int main()
{
    if(Game game(nullptr); game.load())
    {
        while (!game.isDone())
        {
            game.update(0);
        } 
    }  

    return 0;
}