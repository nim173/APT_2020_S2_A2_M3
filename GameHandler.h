#ifndef GAME_HANDLER
#define GAME_HANDLER

#include <string>
#include <sstream>
#include "Types.h"
#include "Player.h"
#include "Game.h"
#include "GameFileHandler.h"

using std::string;

class GameHandler {
public:
    // initialises default mosaic
    GameHandler();

    void playNewGame();

    void loadGame(string fileName, bool testing);
    
    void playGame(int startingRound, int startingPlayer);

    void testGame(string fileName);

    // returns false if EOF encountered while getting user input
    void playTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame);

    // updates player scores and prints round results
    void endRound(bool newGame);

    // repopulates factories and clears player floor line
    // returns the the index of the starting player for the next round
    int resetGameBoard();

    void printGameResults();

    void shuffleTilebag();

private:
    Player* players[NO_OF_PLAYERS];

    LinkedList* tilebag;

    vector<string>* turns; 

    Game* currentGame;

    GameFileHandler* fileHandler;

    // gets initialised by the contents (mozaic format) of the file default.mozaic in constructor
    // used as reference to fill player mosaics
    Mosaic defaultMosaicGrid;

    bool addPlayers();

    bool saveGame();

    // gets turn input and does input error checking (does not check for game rules)
    // returns false if EOF encountered
    bool getPlayerTurn(std::stringstream* stream, int *factoryNo, Tile *tile, int *storageRow, bool newGame);

    bool validateTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame);
    
    void printPlayerPoints(string message);

    void endGame();
};

#endif // GAME_HANDLER
