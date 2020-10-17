#include "AI_Turn.h"
#include "Types.h"

AI_Turn::AI_Turn(int factoryNo, Tile tile, int storageRow, int numTiles, int points, int excessTiles) {
    this->factoryNo = factoryNo;
    this->tile = tile;
    this->storageRow = storageRow;
    this->numTiles = numTiles;
    this->points = points;
    this->excessTiles = excessTiles;
}

int AI_Turn::getExcessTiles() {
    return excessTiles;
}

int AI_Turn::getFactoryNo() {
    return factoryNo;
}

Tile AI_Turn::getTile() {
    return tile;
}

int AI_Turn::getStorageRow() {
    return storageRow;
}

int AI_Turn::getNumTiles() {
    return numTiles;
}

int AI_Turn::getPoints() {
    return points;
}
