#ifndef GAME_HANDLER
#define GAME_HANDLER

#include <string>
#include "Types.h"
#include "Player.h"
#include "Game.h"

class GameHandler {
public:
    // initialises default mosaic
    GameHandler();

    void playNewGame();
    bool loadGame();

private:
    Player* players[NO_OF_PLAYERS];

    // gets initialised by the contents (mozaic format) of the file default.mozaic in constructor
    // used as reference to fill player mosaics
    Mosaic defaultMosaic;

    bool addPlayers();

    bool saveGame();

    // returns false if EOF encountered while getting user input
    bool playTurn(int playerNo, Game* game);

    // gets turn input and does input error checking (does not check for game rules)
    // returns false if EOF encountered
    bool getPlayerTurn(int* factoryNo, Tile* tile, int* storageRow);

    bool validateTurn(int playerNo, Game* game, int factoryNo, Tile tile, int storageRow);
    
    void printPlayerPoints(string message);

    void printRoundResults();

    void printGameResults();
};

#endif // GAME_HANDLER
