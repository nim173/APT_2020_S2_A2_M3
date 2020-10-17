#include <vector>
#include <string>
#include "Game.h"
#include "Types.h"

Game::Game(LinkedList* tilebag, LinkedList* boxLid) {
    // initialize factories
    for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        factories[i] = new vector<Tile>();
    }
    populateFactories(tilebag, boxLid);
}

Game::~Game() {
    // delete factories
     for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        delete factories[i];
    }
}

void Game::populateFactories(LinkedList* tilebag, LinkedList* boxLid) {
    // add first-player tile to centre factory
    factories[CENTER_FACTORY]->push_back('F');

    for (int i = 1; i < NO_OF_FACTORIES; ++i) {
        for (int j = 0; j < FACTORY_MAX_SIZE; ++j) {
            if (tilebag->getSize() == 0) {
                int boxLidSize = boxLid->getSize();
                for (int k = 0; k < boxLidSize; k++) {
                    tilebag->addBack(boxLid->removeFront());
                }
            } // add from box lid if tile bag is empty
            if (tilebag->getSize() != 0) {
                factories[i]->push_back(tilebag->removeFront());
            } // In the rare case that both tilebag and box lid is empty, stop filling the factories and continue the round
        }
    }
}

//move all tiles in a factory to the center
void Game::addToCentreFactory(int factoryNo){
    for(unsigned int i = 0; i< (factories[factoryNo])->size(); i++){
        factories[CENTER_FACTORY]->push_back(factories[factoryNo]->at(i));
    }
    //after adding all items to center, clear from factory.
    factories[factoryNo]->clear();
}

bool Game::roundOver() {
    bool result = true;
    for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        if (!factories[i]->empty()) {
            result = false;
            // stop the loop
            i = NO_OF_FACTORIES;
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
            *errorMessage += "No " + std::string(1, tile) + " tiles in factory " 
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
            result += std::string(1, factories[i]->at(j)) + " ";
        }

        result += "\n";
    }
    return result;
}

bool Game::checkForFirstPlayerTile(bool AImode) {
    bool result = false;
    if (!factories[0]->empty()) {
        if (factories[0]->at(0) == FIRST_PLAYER_TILE) {
            if (!AImode) {
                factories[0]->erase(factories[0]->begin());
            }
            result = true;
        }
    } 
    return result;
}

int Game::removeFromFactory(int factoryNo, Tile tile) {
    int result = 0;
    vector<Tile>::iterator it;
    for(it = factories[factoryNo]->begin(); it != factories[factoryNo]->end(); ++it)    {
        if (*it == tile) {
            factories[factoryNo]->erase(it);
            --it;
            ++result;
        }
    }
    // add the remaining tiles in this factory to centre factory
    if (factoryNo != CENTER_FACTORY) {
        addToCentreFactory(factoryNo);
    }
    return result;
}

int Game::getNumberOfTiles(int factoryNo, Tile tile) {
    int result = 0;
    for (unsigned int i = 0; i < factories[factoryNo]->size(); ++i) {
        if (factories[factoryNo]->at(i) == tile) {
            ++result;
        }
    }
    return result;
}
