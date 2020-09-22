#ifndef GAME_FILE_HANDLER
#define GAME_FILE_HANDLER

#include <string>
#include "Player.h"
#include "Types.h"
#include "DELinkedList.h"

using std::string;
using std::vector;

class GameFileHandler {
public:
    GameFileHandler();
    ~GameFileHandler();

    bool loadTileBag(string file, LinkedList* tilebag);
    bool loadMosaic(string file, Mosaic mosaic);

    void save();
    void load();

private:



private:
    std::ifstream inFile;
};


#endif // GAME_FILE_HANDLER