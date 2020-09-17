#include <string>
#include "Game.h"

using std::string;

Game::Game(string player1Name, string player2Name) {
    players[0] = new Player(player1Name);
    players[1] = new Player(player2Name);
}

Game::~Game() {
    // delete player1;
    // delete player2;
}

bool Game::playTurn(int player, int factory, char tile, int storageRow) {
    // remove tile(s) from relevant factory, obtain number of tiles removed - Game.removeFromFactory(int factory, char tile)
    // add tile(s) to player storage row - Player.addToStorageRow(int row, char tile) & Player.addBrokenTiles(char tile)

    // handle 'f' tile 
    // if first element of centre factory is f, add f to broken tile vector of player - Player.addBrokenTiles('f')
    return false;
}
