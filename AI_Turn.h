#ifndef AI_TURN
#define AI_TURN

#include "Types.h"

class AI_Turn {
public:
    AI_Turn(int factoryNo, Tile tile, int storageRow, int numTiles, int points, int excessTiles);

    int getFactoryNo();
    Tile getTile();
    int getStorageRow();
    int getNumTiles();
    int getPoints();
    int getExcessTiles();

private:
    int factoryNo;
    Tile tile;
    int storageRow;
    int numTiles;
    int points;
    int excessTiles;
};

#endif // AI_TURN
