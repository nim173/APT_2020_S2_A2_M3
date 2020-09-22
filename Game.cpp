#include <vector>
#include <string>
#include "Game.h"
#include "Types.h"

Game::Game(LinkedList* tilebag) {
    // initialize factories
    for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        factories[i] = new vector<Tile>(FACTORY_MAX_SIZE);
    }
    populateFactories(tilebag);
}

Game::~Game() {
    // delete factories
     for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        delete factories[i];
    }
}

void Game::populateFactories(LinkedList* tilebag) {
    // add first-player tile to centre factory
    factories[0]->push_back('F');

    for (int i = 1; i < NO_OF_FACTORIES; ++i) {
        for (int j = 0; j < FACTORY_MAX_SIZE; ++j) {
            factories[i]->push_back(tilebag->removeFront());
        }
    }
}

bool Game::roundOver() {
    bool result = false;
    for (int i = 0; i < NO_OF_FACTORIES && !result; ++i) {
        if (!factories[i]->empty()) {
            result = true;
        }
    }
    return result;
}

bool Game::validateTurn(int factoryNo, Tile tile, string* errorMessage) {
    bool valid = false;
    if (!factories[factoryNo]->empty()) {
        unsigned int i = 0;
        while (i < factories[factoryNo]->size() && !valid) {
            if (factories[factoryNo]->at(i) == tile) {
                valid = true;
            }
            ++i;
        }
        if (!valid) {
            *errorMessage += "No " + std::to_string(tile) + " tiles in factory" 
                          + std::to_string(factoryNo) + "\n";
        }
    } else {
        *errorMessage += "Factory " + std::to_string(factoryNo) + " is empty\n";
    }
    return valid;
}

std::string Game::printFactories() {
    std::string result;
    for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        result += std::to_string(i) + ": ";

        for (unsigned int j = 0; j < factories[i]->size(); ++j) {
            result += factories[i]->at(j) + " ";
        }

        result += "\n";
    }
    return result;
}

bool Game::checkForFirstPlayerTile() {
    bool result = false;
    if (!factories[0]->empty()) {
        if (factories[0]->at(0) == FIRST_PLAYER_TILE) {
            result = true;
        }
    } 
    return result;
}
