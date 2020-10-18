#ifndef PLAYER
#define PLAYER

#include <vector>
#include <string>
#include "Types.h"
#include "DELinkedList.h"

using std::string;
using std::vector;

class Player {
public:
    Player(string name, bool advancedMode);
    ~Player();

    string getName();

    int getPoints();
    void setPoints(int points);

    string printPlayerBoard();

    // Checks if: 1. Given storage row is already full, and
    //            2. Given tile is already filled in the given row of the mosaic, and
    //            3. Given storage row has a different coloured tile,
    // and returns false is any of the above conditions is true
    bool validateTurn(Tile tile, int row, string* errorMessage);

    // adds the specified number of tiles to the specified storage row
    // adds excess tiles to the floor line (broken tiles), 
    // returns the # of remaining tiles if floorline is full
    int addToStorageRow(int row, Tile tile, int numTilesToAdd);

    // add broken tiles to floor line
    int addToFloorLine(Tile tile, int numTilesToAdd);

    // fills wall, and updates player points as per Azul rules, and adds the remaining tiles to the box lid
    int updateScore(Mosaic defaultMosaic, LinkedList* boxLid, bool advancedMode);

    // returns true if player has first-player tile, adds the remaining tiles to the box lid
    bool resetFloorline(LinkedList* boxLid);

    // returns true if specified row is full
    bool isFilled(int row);

    // returns the number of tiles in row
    int numTilesInRow(int row);

    // places the tile in the filled row in given column, returns false if column is not empty
    bool placeTileInWall(int row, int col, Tile *tile);

    // calculates total points generated from adding a tile to a specified position in wall
    int getPointsForAdjacentTiles(LinkedList* boxLid, int row, int col, int mosaicDim, Tile tile, bool AImode);

    void setCpu(bool cpu);
    bool isCpu();

    bool isStorageRowsEmpty();

    // used in AI mode to calculate the possible points from a valid move
    // sets excess to the amount of tiles that would go in to floor line from move
    int getPointsForTurn(Mosaic mosaic, Tile tile, int storageRow, int numTiles, int *excess);
    
private:
    string name;
    int points;
    
    bool advancedMode;

    // if the player is CPU or not, also a check for AI mode
    bool cpu;

    // Player Mosaic
    vector<Tile>** storageRow;
    Mosaic wall;

    LinkedList* floorLine;

    // returns current mosaic dimensions, 6 if in advanced mode, 5 if not
    int getMosaicDim();

    // calculate points to be deducted for tiles in floor line (as per official azul rules)
    int caclulatefloorLinePointDeduction();
};

#endif // Player
