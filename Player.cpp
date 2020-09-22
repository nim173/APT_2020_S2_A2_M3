#include <string>
#include <vector>
#include "Player.h"

using std::vector;

Player::Player(std::string name) {
    this->name = name;
    points = 0;

    // intialize storage rows
    for (int i = 0; i < MOSAIC_DIM; ++i) {
        storageRow[i] = new vector<Tile>;
    }

    // initialize wall
    wall = new Tile*[MOSAIC_DIM];
    for (int i = 0; i < MOSAIC_DIM; ++i) {
         wall[i] = new Tile[MOSAIC_DIM];
    }
    for (int i = 0; i < MOSAIC_DIM; ++i) {
        for (int j = 0; j < MOSAIC_DIM; ++j) {
            wall[i][j] = EMPTY_SLOT;
        }
    }

    //intitialize floor line
    floorLine = new LinkedList();
}

Player::~Player() {
    // delete storage rows
    for (int i = 0; i < MOSAIC_DIM; ++i) {
        delete storageRow[i];
    }

    // delete wall
    for (int i = 0; i != MOSAIC_DIM; ++i) {
        delete[] wall[i];
    }
    delete[] wall;

    delete floorLine;
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
            result += storageRow[i]->at(j) + " ";
        }

        // partition
        result += "||";

        // add mosaic
        for (int j = 0; j < MOSAIC_DIM; ++j) {
            result += " " + wall[i][j];
        }

        result += "\n";
    }

    // add broken tiles
    result += "broken: ";
    result += floorLine->toString();

    result += "\n";
    return result;
}

bool Player::validateTurn(Tile tile, int row, string* errorMessage) {
    bool valid = true;
    if (!(storageRow[row]->size() == (unsigned int) row)) {
        for (int i = 0; i < MOSAIC_DIM; ++i) {
            if (wall[row][i] == tile) {
                valid = false;
                *errorMessage += "Tile " + std::to_string(tile) + " already filled in row " + 
                                    std::to_string(row) + " of Mosaic\n";
            }
        }
        if (valid) {
            // checks if the first filled slot in the storage row is filled by the same tile or empty
            Tile ch = storageRow[row]->at(0);
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

void Player::addToStorageRow(int row, Tile tile, int numTilesToAdd) {
    for (int i = storageRow[row]->size(); i < row && numTilesToAdd > 0; ++i) {
        storageRow[row]->push_back(tile);
        --numTilesToAdd;
    }
    addToFloorLine(tile, numTilesToAdd);
}

void Player::addToFloorLine(Tile tile, int numTilesToAdd) {
    while (numTilesToAdd > 0) {
        floorLine->addBack(tile);
        --numTilesToAdd;
    }
}

void Player::addToWall(int row, int col, Tile tile) {
    wall[row][col] = tile;
}

int Player::updateScore(Mosaic defaultMosaic) {
    // Go through pattern lines from top to bottom   
    int pointsToAdd = 0;
    for (unsigned int i = 0; i < MOSAIC_DIM; ++i) {
        if (storageRow[i]->size() == (i+1)) {
            ++pointsToAdd;

            // For each complete line add a tile of the same color in the corresponding line of the wall
            for (int j = 0; j < MOSAIC_DIM; ++j) {
                if (storageRow[i]->at(0) == defaultMosaic[i][j]) {
                    wall[i][j] = storageRow[i]->at(0);
                    
                    // Each time a tile is added to the wall, score points immediately (as per official rules)
                    // check for vertically adjacent tiles
                    for (int k = i-1; k >= 0; --k) {
                        if (wall[k][j] != EMPTY_SLOT) {
                            ++pointsToAdd;
                        } else {
                            // stops the loop
                            k = -1;
                        }
                    }
                    for (int k = i; k < MOSAIC_DIM; ++k) {
                        if (wall[k][j] != EMPTY_SLOT) {
                            ++pointsToAdd;
                        } else {
                            // stops the loop
                            k = MOSAIC_DIM;
                        }
                    }

                    // check for horizontally adjacent tiles
                    for (int k = j-1; k >= 0; --k) {
                        if (wall[i][k] != EMPTY_SLOT) {
                            ++pointsToAdd;
                        } else {
                            // stops the loop
                            k = -1;
                        }
                    }
                    for (int k = j; k < MOSAIC_DIM; ++k) {
                        if (wall[i][k] != EMPTY_SLOT) {
                            ++pointsToAdd;
                        } else {
                            // stops the loop
                            k = MOSAIC_DIM;
                        }
                    }

                    // stops the loop
                    j = MOSAIC_DIM;
                }
            }
        }
    }

    // decrease points for tiles in floor line (as per official azul rules)
    int floorLineSize = floorLine->getSize();
    if (floorLineSize >= 0 && floorLineSize < 3) {
        pointsToAdd -= floorLineSize;
    } else if (floorLineSize >= 3 && floorLineSize < 6) {
        pointsToAdd -= (2*floorLineSize - 2);
    } else if (floorLineSize == 6) {
        pointsToAdd -= 11;
    } else {
        pointsToAdd -= 14;
    } // for 7 (or more) tiles

    this->points += pointsToAdd;
    if (this->points < 0) {
        // player score cannot be less than zero
        this->points = 0;
    }

    return pointsToAdd;
}
