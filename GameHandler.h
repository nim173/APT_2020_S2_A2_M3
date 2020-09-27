#ifndef GAME_HANDLER
#define GAME_HANDLER

#include <string>
#include "Types.h"
#include "Player.h"
#include "Game.h"
#include "GameFileHandler.h"

class GameHandler {
public:
    // initialises default mosaic
    GameHandler();

    void playNewGame();
    void loadGame();

private:
    Player* players[NO_OF_PLAYERS];

    LinkedList* tilebag;

    vector<string>* turns; 

    GameFileHandler* fileHandler;

    // gets initialised by the contents (mozaic format) of the file default.mozaic in constructor
    // used as reference to fill player mosaics
    Mosaic defaultMosaicGrid;

    bool addPlayers();

    bool saveGame();

    // returns false if EOF encountered while getting user input
    bool playTurn(int playerNo, Game* game);

    // gets turn input and does input error checking (does not check for game rules)
    // returns false if EOF encountered
    bool getPlayerTurn(int* factoryNo, Tile* tile, int* storageRow);

    bool validateTurn(int playerNo, Game* game, int factoryNo, Tile tile, int storageRow);

    // updates player scores and prints round results
    void endRound();
    
    // repopulates factories and clears player floor line
    // returns the the index of the starting player for the next round
    int resetGameBoard(Game* game);
    
    void printPlayerPoints(string message);

    void printGameResults();
};

#endif // GAME_HANDLER
