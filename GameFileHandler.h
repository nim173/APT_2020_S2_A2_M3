<<<<<<< HEAD
#ifndef GAME_FILE_HANDLER
#define GAME_FILE_HANDLER

#include <string>
#include "Types.h"

using std::string;

class GameFileHandler
{
public:
    GameFileHandler();
    ~GameFileHandler();

    void save(string fileName, string inputFactory, string inputTile, string inputStorageRow );
    void load();

private:
   
};

=======
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


>>>>>>> aedcc130528e470d646fb82c12e5870018348ad0
#endif // GAME_FILE_HANDLER