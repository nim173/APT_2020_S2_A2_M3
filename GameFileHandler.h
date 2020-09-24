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

    void saveGame(string fileName, Player *players[NO_OF_PLAYERS], vector<string> *turns);

    void loadGame(string fileName);

    bool loadTileBag(DEFAULT_TILEBAG_FILE, LinkedList tilebag);

    void loadMosaic(DEFAULT_MOSAIC_FILE, Tile *defaultMosaicGrid);

private:
};

#endif // GAME_FILE_HANDLER
