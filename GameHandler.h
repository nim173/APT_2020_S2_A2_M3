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
    ~GameHandler();

    void playNewGame(bool fixedSeed, int seed, bool advancedMode, bool AImode);

    void loadGame(string fileName, bool testing);
    
    void playGame(int startingRound, int startingPlayer, bool advancedMode, bool AImode);

    void testGame(string fileName);

    // returns false if EOF encountered while getting user input
    void playTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame);

    // updates player scores and prints round results
    void endRound(bool newGame, bool advancedMode);

    // repopulates factories and clears player floor line
    // returns the the index of the starting player for the next round
    int resetGameBoard();

    void printGameResults();

private:
    Player* players[NO_OF_PLAYERS];

    LinkedList* tilebag;
    LinkedList* boxLid;

    vector<string>* turns; 

    Game* currentGame;

    GameFileHandler* fileHandler;

    // gets initialised by the contents (mozaic format) of the file default.mozaic in constructor
    // used as reference to fill player mosaics
    Mosaic defaultMosaicGrid;

    bool addPlayers(bool advancedMode, bool AImode);

    bool saveGame();

    // gets turn input and does input error checking (does not check for game rules)
    // returns false if EOF encountered
    bool getPlayerTurn(std::stringstream* stream, int *factoryNo, Tile *tile, int *storageRow,
                         bool newGame, bool advancedMode, bool AImode);

    bool validateTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame);
    
    void printPlayerPoints(string message);

    void endGame();

    void shuffleTilebag(bool fixedSeed, int seed);

    bool advancedModeWallTiling();

    bool getPlayerWallTilePlacement(std::stringstream *stream, int *wallCol, bool newGame);

    void getAIturn(int *factoryNo, Tile *tile, int *storageRow);
};

#endif // GAME_HANDLER
