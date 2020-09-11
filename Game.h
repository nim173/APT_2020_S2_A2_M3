#ifndef GAME
#define GAME

#include <string>
#include "Player.h"

using std::string;

class Game {
public:
    Game(string player1Name, string player2Name);
    ~Game();

private:
    // We could use a player array of length 2 here, what do you guys think? - N
    Player* player1;
    Player* player2;

    // Tile bag
    // (Shared) Factories
    // Turns - we could store it here, maybe a 2D array for the 5 rounds (or store them in Player)
    // ...
};

#endif // GAME