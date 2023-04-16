#include <SFML/Graphics.hpp>
#include "Snake.h"


int main()
{
    Game game;
    

    while (game.GetWindow().isOpen()) {

        game.HandleInput();
        game.GetElapsed();
        game.Update();
        game.Render();
        game.RestartClock();
    }

    return 0;
}