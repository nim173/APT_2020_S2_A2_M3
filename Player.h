#ifndef PLAYER
#define PLAYER

#include <string>
#include <vector>
#include "Types.h"

using std::string;
using std::vector;

class Player {
public:
    Player(string name);
    ~Player();

    bool addToStorageRow(int row, char tile);
    void addBrokenTiles(char tile);
    void addToMosaic(int row, int col, char tile);
private:
    string name;
    int points;

    // Player Mosaic
    char* storageRow[MOSAIC_DIM];
    char mosaic[MOSAIC_DIM][MOSAIC_DIM];
    vector<char> brokenTiles;
};

#endif // Player
