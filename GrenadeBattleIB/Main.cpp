#include <SFML/Graphics.hpp>
#include "Game.h"
#include <stdlib.h>
#include <time.h>
#include "AssetManager.h"


int main()
{
    // Initialise the random number generator
    srand(time(NULL));

    // Create the game
    Game gameInstance; // default constructor

    // This will not end until the game is over
    gameInstance.RunGameLoop();

    // If we get here, the loop exited, so the game is over

    // Clean up assets
    AssetManager::DestroyAllAssets();

    // End the program by returning
    return 0;
}