#ifndef GAME
#define GAME

#include <string>
#include <vector>
#include "Player.h"
#include "Types.h"

using std::string;
using std::vector;

class Game {
public:
    Game(string player1Name, string player2Name);
    ~Game();

    void addToCentreFactory(int factory);

    bool playTurn(int player, int factory, char tile, int storageRow);

    // checks if all tiles are taken
    // used in main to check end of round (or where game loop is to be run)
    bool roundOver();

    // returns a formatted string showing the winner, final scores, and mosaics
    string getGameResults();

private:
    // We could use a player array of length 2 here, what do you guys think? - N
    Player* players[NO_OF_PLAYERS];

    // Tile bag

    // an array of vectors for the the central factory + 5 factories
    vector<char> factories;

    // Turns - we could store it here, maybe a 2D array for the 5 rounds (or store them in Player)
    vector<string> turns;

    // gets initialised by the contents (mozaic format) of the file default.mozaic in constructor
    // used as reference to fill player mosaics
    char defaultMosaic[MOSAIC_DIM][MOSAIC_DIM];

    // removes the specified tile from the specified factory and returns the number of instances of the tile found
    int removeFromFactory(int factory, char tile);

    // returns a formatted string 
    string getRoundResults();
};

#endif // GAME
