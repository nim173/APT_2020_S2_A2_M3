#include <string>
#include <vector>
#include "Player.h"
#include "Types.h"

using std::vector;

Player::Player(std::string name, bool advancedMode) {
    this->name = name;
    points = 0;
    this->advancedMode = advancedMode;
    cpu = false;

    // set mosaic dimensions depending on type of game play
    int mosaicDim = getMosaicDim();

    // intialize storage rows
    storageRow = new vector<Tile>*[mosaicDim];
    for (int i = 0; i < mosaicDim; ++i) {
        storageRow[i] = new vector<Tile>();
    }
    for (int i = 0; i < mosaicDim; ++i) {
        for (int j = 0; j < i+1; ++j) {
            storageRow[i]->push_back(EMPTY_SLOT);
        }
    }

    // initialize wall
    wall = new Tile*[mosaicDim];
    for (int i = 0; i < mosaicDim; ++i) {
         wall[i] = new Tile[mosaicDim];
    }
    for (int i = 0; i < mosaicDim; ++i) {
        for (int j = 0; j < mosaicDim; ++j) {
            wall[i][j] = EMPTY_SLOT;
        }
    }

    //intitialize floor line
    floorLine = new LinkedList();
}

Player::~Player() {
    int mosaicDim = getMosaicDim();
    // delete storage rows
    for (int i = 0; i < mosaicDim; ++i) {
        if (storageRow[i] != nullptr) {
            delete storageRow[i];
            storageRow[i] = nullptr;
        }
    }
    delete[] storageRow;
    storageRow = nullptr;

    // delete wall
    for (int i = 0; i != mosaicDim; ++i) {
        delete wall[i];
        wall[i] = nullptr;
    }
    delete[] wall;
    wall = nullptr;

    delete floorLine;
    floorLine = nullptr;
}

std::string Player::getName() {
    return name;
}

int Player::getPoints() {
    return points;
}

void Player::setPoints(int points) {
    this->points = points;
}

std::string Player::printPlayerBoard() {
    std::string result;
    int mosaicDim = getMosaicDim();
    
    // print storage rows and mosaic
    for (int i = 0; i < mosaicDim; ++i) {
        // row number
        result += std::to_string(i + 1) + ": ";

        // add empty space
        for (int j = 0; j < mosaicDim - i - 1; ++j) {
            result += "  ";
        }

        // add storage rows (in reverse)
        for (int j = i; j >= 0; --j) {
            result += std::string(1, storageRow[i]->at(j)) + " ";
        }

        // partition
        result += "||";

        // add mosaic
        for (int j = 0; j < mosaicDim; ++j) {
            result += " " + std::string(1, wall[i][j]);
        }

        result += "\n";
    }

    // add broken tiles
    result += "broken: ";
    std::string floorLineString = floorLine->toString();
    if (floorLineString != "") {
        std::string floorLineWithSpacesString( 2 * floorLineString.size() - 1, ' ' );
        for (unsigned int i = 0, j = 0; i < floorLineString.length(); i++, j+=2) {
            floorLineWithSpacesString[j] = floorLineString[i];
        }
        result += floorLineWithSpacesString;
    }

    result += "\n";
    return result;
}

bool Player::validateTurn(Tile tile, int row, string* errorMessage) {
    bool valid = true;
    int mosaicDim = getMosaicDim();

    if (row != mosaicDim+1) {
        // checks if the first filled slot in the storage row is filled by the same tile or empty
        Tile ch = storageRow[row-1]->at(0);
        if (ch == tile) {
            // check if storage is full 
            valid = false;
            for (int i = 0; i < row; ++i) {
                if (storageRow[row-1]->at(i) == EMPTY_SLOT) {
                    valid = true;
                    // stop the loop
                    i = row;
                }
            }
            if (!valid) {
                *errorMessage += "Row " + std::to_string(row) + " is already full\n";
            }
        } else if (ch == EMPTY_SLOT) {
            // check if corresponding mosaic is already filled
            for (int i = 0; i < mosaicDim; ++i) {
                if (wall[row-1][i] == tile) {
                    valid = false;
                    *errorMessage += "Tile " + std::string(1, tile) + " already filled in row " + 
                                        std::to_string(row) + " of Mosaic\n";
                    // stop the loop
                    i = mosaicDim;
                }
            }
        } else {
            valid = false;
            *errorMessage += "Storage Row " + std::to_string(row) + 
                        " already holds tiles.\nYou may only add tiles of the same color to it\n";
        }
    } // if selected row is not the floor line
    return valid;
}

int Player::addToStorageRow(int row, Tile tile, int numTilesToAdd) {
    if (row != getMosaicDim()+1) {
        for (int i = 0; i < row && numTilesToAdd > 0; ++i) {
            if (storageRow[row-1]->at(i) == EMPTY_SLOT) {
                storageRow[row-1]->at(i) = tile;
                --numTilesToAdd;
            }
        }
    } // if selected row is not the floor line
    return addToFloorLine(tile, numTilesToAdd);
}

int Player::addToFloorLine(Tile tile, int numTilesToAdd) {
    int floorLineMaxSize = 0;
    if (!advancedMode) {
        floorLineMaxSize = FLOOR_LINE_MAX_SIZE;
    } else {
        floorLineMaxSize = ADV_FLOOR_LINE_MAX_SIZE;
    }
    while (numTilesToAdd > 0 && floorLine->getSize() < floorLineMaxSize) {
        floorLine->addBack(tile);
        --numTilesToAdd;
    }
    return numTilesToAdd;
}

int Player::updateScore(Mosaic defaultMosaic, LinkedList* boxLid, bool advancedMode) {
    int pointsToAdd = 0;
    if (!advancedMode) {
        int mosaicDim = getMosaicDim();
        // Go through pattern lines from top to bottom   
        Tile temp;
        for (int i = 0; i < mosaicDim; ++i) {
            // check if last tile of a row is completed
            temp = storageRow[i]->at(i);
            if (temp != EMPTY_SLOT) {
                // For each complete line add a tile of the same color in the corresponding line of the wall
                for (int j = 0; j < mosaicDim; ++j) {
                    if (temp == defaultMosaic[i][j]) {
                        wall[i][j] = temp;
                        pointsToAdd += getPointsForAdjacentTiles(boxLid, i, j, mosaicDim, temp, false);
                        // stops the loop
                        j = mosaicDim;
                    }
                }
            } // if a row is completed
        }
    }

    // decrease points for tiles in floor line (as per official azul rules)
    pointsToAdd -= caclulatefloorLinePointDeduction();

    // set points    
    this->points += pointsToAdd;
    if (this->points < 0) {
        // player score cannot be less than zero
        this->points = 0;
    }

    return pointsToAdd;
}

int Player::getPointsForAdjacentTiles(LinkedList* boxLid, int row, int col, 
            int mosaicDim, Tile tile, bool AImode) {
    // each tile placement in wall scores a minimum of 1 point
    int pointsToAdd = 0;
    // add all tiles from any pattern lines that now have no tile in the rightmost space to tilebag
    if (!AImode) {
        for (int k = 0; k < row; ++k) {
            boxLid->addBack(tile);
        }
        for (int k = 0; k <= row; ++k) {
            storageRow[row]->at(k) = EMPTY_SLOT;
        }
    }
                        
    // Each time a tile is added to the wall, score points immediately (as per official rules)
    // check for vertically adjacent tiles
    bool verticallyAdjacentTilesFound = false;
    for (int k = row-1; k >= 0; --k) {
        if (wall[k][col] != EMPTY_SLOT) {
            verticallyAdjacentTilesFound = true;
            ++pointsToAdd;
        } else {
            // stops the loop
            k = -1;
        }
    }
    for (int k = row+1; k < mosaicDim; ++k) {
        if (wall[k][col] != EMPTY_SLOT) {
            verticallyAdjacentTilesFound = true;
            ++pointsToAdd;
        } else {
            // stops the loop
            k = mosaicDim;
        }
    }
    // if vertically adjacent tiles found, add 1 more point
    if (verticallyAdjacentTilesFound) {
        ++pointsToAdd;
    } 

    // check for horizontally adjacent tiles
    bool horizontallyAdjacentTilesFound = false;
    for (int k = col-1; k >= 0; --k) {
        if (wall[row][k] != EMPTY_SLOT) {
            horizontallyAdjacentTilesFound = true;
            ++pointsToAdd;
        } else {
            // stops the loop
            k = -1;
        }
    }
    for (int k = col+1; k < mosaicDim; ++k) {
        if (wall[row][k] != EMPTY_SLOT) {
            horizontallyAdjacentTilesFound = true;
            ++pointsToAdd;
        } else {
            // stops the loop
            k = mosaicDim;
        }
    }
    // if horizontally adjacent tiles found, add 1 more point
    if (horizontallyAdjacentTilesFound) {
        ++pointsToAdd;
    }

    // if no vertically and horizontally adjacent tiles found, add 1 point
    if (!verticallyAdjacentTilesFound && !horizontallyAdjacentTilesFound) {
        ++pointsToAdd;
    }

    return pointsToAdd;
}

int Player::caclulatefloorLinePointDeduction() {
    // see Azul game rules
    int pointsToDeduct = 0;
    int floorLineSize = floorLine->getSize();
    if (floorLineSize >= 0 && floorLineSize < 3) {
        pointsToDeduct += floorLineSize;
    } else if (floorLineSize >= 3 && floorLineSize < 6) {
        pointsToDeduct += (2*floorLineSize - 2);
    } else if (floorLineSize == 6) {
        pointsToDeduct += 11;
    } else if (floorLineSize == 7) {
        pointsToDeduct += 14;
    } else {
        pointsToDeduct += 18;
    } // for 8 (or more) tiles
    return pointsToDeduct;
}

bool Player::resetFloorline(LinkedList* boxLid) {
    bool result = false;
    Tile temp;
    while (floorLine->getSize() > 0) {
        temp = floorLine->removeFront();
        if (temp == FIRST_PLAYER_TILE) {
            result = true;
        } else {
            boxLid->addBack(temp);
        }
    }
    return result;
}

int Player::getMosaicDim() {
    int mosaicDim = 0;
    if (!advancedMode) {
        mosaicDim = MOSAIC_DIM; 
    } else {
        mosaicDim = ADV_MOSAIC_DIM;
    }
    return mosaicDim;
}

bool Player::isFilled(int row) {
    bool result = false;
    int mosaicDim = getMosaicDim();
    if (row >= 0 && row < mosaicDim) {
        if (storageRow[row]->at(row) != EMPTY_SLOT) {
            result = true;
        } // if a row is completed
    }
    return result;
}

int Player::numTilesInRow(int row) {
    int result = 0;
    int mosaicDim = getMosaicDim();
    if (row >= 0 && row < mosaicDim) {
        while (result <= row && storageRow[row]->at(result) != EMPTY_SLOT) {
            ++result;
        }
    }
    return result;
}

bool Player::placeTileInWall(int row, int col, Tile *tile) {
    bool result = true;
    if (wall[row][col] == EMPTY_SLOT) {
        Tile temp = storageRow[row]->at(row);
        wall[row][col] = temp;
        *tile = temp;
    } else {
        result = false;
    }
    return result;
}

void Player::setCpu(bool cpu) {
    this->cpu = cpu;
}

bool Player::isCpu() {
    return cpu;
}

int Player::getPointsForTurn(Mosaic mosaic, Tile tile, int storageRow, int numTiles, int *excess) {
    int result = 0;
    // get number of empty slots in row
    int emptySlots = 0;
    if (storageRow != FLOOR_LINE_INDEX) {
        for (int i = 0; i < storageRow+1; i++) {
            if (this->storageRow[storageRow]->at(i) == EMPTY_SLOT) {
                ++emptySlots;
            }
        }
        if (numTiles >= emptySlots) {
            // find slot in wall for tile
            for (int i = 0; i < MOSAIC_DIM; i++) {
                if (mosaic[storageRow][i] == tile) {
                    result += getPointsForAdjacentTiles(nullptr, storageRow, i, MOSAIC_DIM, tile, true);
                    // stop the loop
                    i = MOSAIC_DIM;
                }
            }
        }
    }
    if (numTiles > emptySlots) {
        // decrease points according to floor line rules
        if (floorLine->getSize() < FLOOR_LINE_MAX_SIZE) {
            numTiles -= emptySlots;
            *excess += numTiles;
            if (FLOOR_LINE_MAX_SIZE - floorLine->getSize() < *excess) {
                *excess = FLOOR_LINE_MAX_SIZE - floorLine->getSize();
            }
            int temp = *excess;
            for (int i = floorLine->getSize(); i < FLOOR_LINE_MAX_SIZE && temp != 0; ++i) {
                if (i >= 0 && i < 2) {
                    result -= 1;
                } else if (i >= 2 && i < 5) {
                    result -= 2;
                } else if (i >= 5 && i < 7) {
                    result -= 3;
                }
                --temp;
            }
        } // if floor line is not full
    } // if there is excess tiles
    
    return result;
}

bool Player::isStorageRowsEmpty() {
    bool result = true;
    for (int i = 0; i < getMosaicDim(); ++i) {
        if (storageRow[i]->at(0) != EMPTY_SLOT) {
            result = false;
            // stop the loop
            i = getMosaicDim();
        }
    }
    return result;
}
