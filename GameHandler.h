#ifndef GAME_HANDLER
#define GAME_HANDLER

#include <string>
#include "Types.h"
#include "Player.h"
#include "Game.h"

class GameHandler {
public:
    void playNewGame();
    bool loadGame();

private:
    Player* players[NO_OF_PLAYERS];

    bool addPlayers();

    bool saveGame();

    bool playTurn(int playerNo, Game* game);

    void printRoundResults();

    void printGameResults();
};

#endif // GAME_HANDLER
