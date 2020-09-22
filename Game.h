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
    Game(LinkedList* tilebag); 
    ~Game();

    // called in the constructor and at the end of each round to populate factories from tilebag
    void populateFactories(LinkedList* tilebag);

    // removes the specified tile from the specified factory and returns the number of instances of the tile found
    // assume specified factory is not empty
    int removeFromFactory(int factoryNo, Tile tile); // - Ryan
    
    // checks if all tiles (in factories) are taken
    bool roundOver();

    // returns false if the given factory is empty or if it does not contain the specified tile
    bool validateTurn(int factoryNo, Tile tile, string* errorMessage);

    // returns true if the FIRST_PLAYER_TILE is not allocated to a player
    // (i.e. it is at the first position of the centre factory [0])
    bool checkForFirstPlayerTile();

    std::string printFactories();

private:
    // an array of vectors for the the central factory + 5 factories
    vector<Tile>* factories[NO_OF_FACTORIES];

    // add all tiles in given factory to the first vector of factories array (i.e. Centre Factory) - Ryan
    void addToCentreFactory(int factoryNo); 
};

#endif // GAME
