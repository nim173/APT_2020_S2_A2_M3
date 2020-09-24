#include <vector>
#include <string>
#include "Game.h"
#include "Types.h"

Game::Game(LinkedList* tilebag) {
    // initialize factories
    for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        factories[i] = new vector<Tile>();
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
    factories[CENTER_FACTORY]->push_back('F');

    for (int i = 1; i < NO_OF_FACTORIES; ++i) {
        for (int j = 0; j < FACTORY_MAX_SIZE; ++j) {
            factories[i]->push_back(tilebag->removeFront());
        }
    }
}

// int Game::removeFromFactory(int factoryNo, Tile tile){
//     int toRemove[5] = {0};
//     int count = 0;

//     for(int i=0 ; i < (factories[factoryNo])->size(); i++){
//         if((factories[factoryNo]->at(i)) == tile){
//             toRemove[count] = i;
//             count++;
//         }
//     }

//     for(int i = 0; i< count; i++)
//     {
//         factories[factoryNo]->erase(factories[factoryNo]->begin() + toRemove[i]);
//     }
//     //return how many tiles were found.
//     return count;
// }

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

bool Game::checkForFirstPlayerTile() {
    bool result = false;
    if (!factories[0]->empty()) {
        if (factories[0]->at(0) == FIRST_PLAYER_TILE) {
            factories[0]->erase(factories[0]->begin());
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