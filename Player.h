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
    bool validateTurn(Tile tile, int row, string* errorMessage);

    // adds the specified number of tiles to the specified storage row
    // adds excess tiles to the floor line (broken tiles)
    void addToStorageRow(int row, Tile tile, int numTilesToAdd);

    // add broken tiles to floor line
    void addToFloorLine(Tile tile, int numTilesToAdd);

    // add specified tile to given position in mosaic
    void addToWall(int row, int col, Tile tile);

    int updateScore(Mosaic defaultMosaic);
    
private:
    string name;
    int points;

    // Player Mosaic
    vector<Tile> storageRow[MOSAIC_DIM]; // changing this to LinkedList after size field added to DELinkedList 
    Mosaic wall;

    // the floor line would not have a max size, as box-lid is out of scope and to keep track of the FIRST_PLAYER_TILE
    // for scoring only the first 7 tiles would be counted as per game rules
    vector<Tile> floorLine; // TODO - change to linkedlist
};

#endif // Player
