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

    void testGame(string fileName);

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

    // play game in the specified starting round with specified starting player in the specified mode
    void playGame(int startingRound, int startingPlayer, bool advancedMode, bool AImode);

    bool saveGame();

    void endGame();

    // returns false if EOF encountered while getting user input
    // does not print error messages if not in a new game
    void playTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame);

    // updates player scores and prints round results
    void endRound(bool newGame, bool advancedMode);

    // repopulates factories and clears player floor line
    // returns the the index of the starting player for the next round
    int resetGameBoard();

    void printGameResults();
    
    // adds players to game, returns false if EOF detected
    bool addPlayers(bool advancedMode, bool AImode);

    // gets turn input and does input error checking (does not check for game rules)
    // returns false if EOF encountered
    bool getPlayerTurn(std::stringstream* stream, int *factoryNo, Tile *tile, int *storageRow,
                         bool newGame, bool advancedMode, bool AImode);

    // returns false if turn chosen is invalud with respect to game context
    // does not print error messages if not in a new game
    bool validateTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame);
    
    void printPlayerPoints(string message);

    void shuffleTilebag(bool fixedSeed, int seed);

    // handles the wall tiling phase in advanced mode, returns false if EOF
    bool advancedModeWallTiling();

    // used by advancedModeWallTiling() to get player command for wall tiling
    bool getPlayerWallTilePlacement(std::stringstream *stream, int *wallCol, bool newGame);

    // used in AI mode to determine next move
    void getAIturn(int *factoryNo, Tile *tile, int *storageRow);
};

#endif // GAME_HANDLER
