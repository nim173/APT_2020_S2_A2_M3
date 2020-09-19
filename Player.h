#ifndef PLAYER
#define PLAYER

#include <vector>
#include <string>
#include "Types.h"

using std::string;
using std::vector;

class Player {
public:
    Player(string name);
    ~Player();

    string getName();
    int getPoints();

    string printPlayerBoard();

    // Checks if: 1. Given storage row is already full, and
    //            2. Given tile is already filled in the given row of the mosaic, and
    //            3. Given storage row has a different coloured tile,
    // and returns false is any of the above conditions is true
    bool validateTurn(char tile, int row, string* errorMessage);

    // adds the specified number of tiles to the specified storage row
    // adds excess tiles to the floor line (broken tiles)
    // if floor line is full, returns the number of excess tiles 
    // (so that they could be added to the back of the tilebag)
    int addToStorageRow(int row, char tile, int numTilesToAdd);

    // add broken tiles to floor line
    void addToFloorLine(char tile);

    void addToMosaic(int row, int col, char tile);
    
private:
    string name;
    int points;

    // Player Mosaic
    vector<char> storageRow[MOSAIC_DIM];
    char mosaic[MOSAIC_DIM][MOSAIC_DIM];
    vector<char> brokenTiles;
};

#endif // Player
