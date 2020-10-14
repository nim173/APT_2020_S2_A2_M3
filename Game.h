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
    Game(LinkedList* tilebag, LinkedList* boxLid); 
    ~Game();

    // called in the constructor and at the end of each round to populate factories from tilebag
    void populateFactories(LinkedList* tilebag, LinkedList* boxLid);

    // removes the specified tile from the specified factory and returns the number of instances of the tile found
    // and adds the remaining tiles to centre factory (if not rmeoving from centre factory)
    // assume specified factory is not empty
    int removeFromFactory(int factoryNo, Tile tile);
    
    // checks if all tiles (in factories) are taken
    bool roundOver();

    // returns false if the given factory is empty or if it does not contain the specified tile
    bool validateTurn(int factoryNo, Tile tile, string* errorMessage);

    // returns true and removes the  FIRST_PLAYER_TILE if it is at the first position of the centre factory
    bool checkForFirstPlayerTile();

    std::string printFactories();

private:
    // an array of vectors for the the central factory + 5 factories
    vector<Tile>* factories[NO_OF_FACTORIES];

    // add all tiles in given factory to the first vector of factories array (i.e. Centre Factory) - Ryan
    void addToCentreFactory(int factoryNo); 
};

#endif // GAME
