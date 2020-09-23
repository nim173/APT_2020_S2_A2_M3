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

#endif // GAME_FILE_HANDLER