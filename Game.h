#ifndef GAME
#define GAME

#include <vector>
#include <string>
#include "Player.h"
#include "Types.h"

class Game {
public:
    Game(); // 
    // another constructor with tilebag as a parameter - load game - Sean
    ~Game();

    // removes the specified tile from the specified factory and returns the number of instances of the tile found
    int removeFromFactory(int factory, char tile); // - Ryan
    
    // checks if all tiles (in factories) are taken
    bool roundOver();

    std::string printFactories();

private:
    // Tile bag - 
    // ...

    // an array of vectors for the the central factory + 5 factories
    std::vector<char> factories[NO_OF_FACTORIES];

    // Turns - we could store it here, maybe a 2D array for the 5 rounds (or store them in Player)
    std::vector<std::string> turns; 

    // gets initialised by the contents (mozaic format) of the file default.mozaic in constructor
    // used as reference to fill player mosaics
    char defaultMosaic[MOSAIC_DIM][MOSAIC_DIM];

    void addToCentreFactory(int factory); // add to the frist vector of factories array - Ryan
};

#endif // GAME
