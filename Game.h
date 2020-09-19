#ifndef GAME
#define GAME

#include <vector>
#include <string>
#include "Player.h"
#include "Types.h"
#include "DELinkedList.h"

using std::string;
using std::vector;

class Game {
public:
    // constructors - loads tilebag, and fills factories
    Game(); // default constructor - loads default tilebag from textfile - Sean
    // another constructor with tilebag as a parameter - load game - Sean
    ~Game();

    // removes the specified tile from the specified factory and returns the number of instances of the tile found
    // assume specified factory is not empty
    int removeFromFactory(int factoryNo, char tile); // - Ryan
    
    // checks if all tiles (in factories) are taken
    bool roundOver();

    // returns false if the given factory is empty or if it does not contain the specified tile
    bool validateTurn(int factoryNo, char tile, string* errorMessage);

    void addToTileBag(char tile);

    char removeFromTileBag();

    // returns true if the FIRST_PLAYER_TILE is not allocated to a player
    // (i.e. it is at the first position of the centre factory [0])
    bool checkForFirstPlayerTile();

    std::string printFactories();

private:
    LinkedList tilebag;

    // an array of vectors for the the central factory + 5 factories
    vector<char> factories[NO_OF_FACTORIES];

    // Turns - we could store it here, maybe a 2D array for the 5 rounds (or store them in Player)
    vector<string> turns; 

    // add all tiles in given factory to the first vector of factories array (i.e. Centre Factory) - Ryan
    void addToCentreFactory(int factoryNo); 
};

#endif // GAME
