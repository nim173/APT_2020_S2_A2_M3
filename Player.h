#ifndef PLAYER
#define PLAYER

#include <vector>
#include <string>
#include "Types.h"

class Player {
public:
    Player(std::string name);
    ~Player();

    std::string getName();
    int getPoints();
    std::string printPlayerBoard(); // Nim
    bool addToStorageRow(int row, char tile);
    void addBrokenTiles(char tile);
    void addToMosaic(int row, int col, char tile);
    
private:
    std::string name;
    int points;

    // Player Mosaic
    std::vector<char> storageRow[MOSAIC_DIM];
    char mosaic[MOSAIC_DIM][MOSAIC_DIM];
    std::vector<char> brokenTiles;
};

#endif // Player
