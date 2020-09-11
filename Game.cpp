#include <string>
#include "Game.h"

using std::string;

Game::Game(string player1Name, string player2Name) {
    player1 = new Player(player1Name);
    player2 = new Player(player2Name);
}

Game::~Game() {
    delete player1;
    delete player2;
}