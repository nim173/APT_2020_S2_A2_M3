#include <vector>
#include "Game.h"
#include "Types.h"

Game::Game() {
    
}

Game::~Game() {
    // delete factories - TODO
}

bool Game::roundOver() {
    bool result = false;
    for (int i = 0; i < NO_OF_FACTORIES && !result; ++i) {
        if (!factories[i].empty()) {
            result = true;
        }
    }
    return result;
}

std::string Game::printFactories() {
    std::string result;
    for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        result += std::to_string(i) + ": ";

        for (unsigned int j = 0; j < factories[i].size(); ++j) {
            result += factories[i].at(j) + " ";
        }

        result += "\n";
    }
    return result;
}