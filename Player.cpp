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

        // add storage rows (in reverse)
        for (int j = i; j >= 0; --j) {
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

bool Player::validateTurn(char tile, int row, string* errorMessage) {
    bool valid = true;
    if (!(storageRow[row].size() == (unsigned int) row)) {
        for (int i = 0; i < MOSAIC_DIM; ++i) {
            if (mosaic[row][i] == tile) {
                valid = false;
                *errorMessage += "Tile " + std::to_string(tile) + " already filled in row " + 
                                    std::to_string(row) + " of Mosaic\n";
            }
        }
        if (valid) {
            // checks if the first filled slot in the storage row is filled by the same tile or empty
            char ch = storageRow[row].at(0);
            if (!(ch == tile || ch == EMPTY_SLOT)) {
                valid = false;
                *errorMessage += "Storage Row " + std::to_string(row) + 
                            " already holds tiles.\nYou may only add tiles of the same color to it\n";
            } else {
                // turn is valid for player
            }
        }
    } else {
        valid = false;
        *errorMessage += "Player storage row " + std::to_string(row) + "is already full\n";
    }
    return valid;
}
