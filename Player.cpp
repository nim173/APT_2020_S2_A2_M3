#include <string>
#include "Player.h"

Player::Player(std::string name) {
    this->name = name;
}

Player::~Player() {

}

std::string Player::getName() {
    return name;
}

int Player::getPoints() {
    return points;
}

std::string Player::printPlayerBoard() {
    std::string result;
    
    // print storage rows and mosaic
    for (int i = 0; i < MOSAIC_DIM; ++i) {
        // row number
        result += std::to_string(i + 1) + ": ";

        // add empty space
        for (int j = 0; j < MOSAIC_DIM - i - 1; ++j) {
            result += "  ";
        }

        // add storage rows
        for (int j = 0; j < i + 1; ++j) {
            result += storageRow[i].at(j) + " ";
        }

        // partition
        result += "||";

        // add mosaic
        for (int j = 0; j < MOSAIC_DIM; ++j) {
            result += " " + mosaic[i][j];
        }

        result += "\n";
    }

    // add broken tiles
    result += "broken: ";
    for (unsigned int i = 0; i < brokenTiles.size(); ++i) {
        result += " " + brokenTiles.at(i);
    }

    result += "\n";
    return result;
}