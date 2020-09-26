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

    bool loadFileCheck(string fileName);

    bool saveFileCHeck(string fileName);

    void saveGame(string fileName, LinkedList tileBag, Player *players[NO_OF_PLAYERS], vector<string> *turns);

    void loadGame(string fileName);

    bool loadTileBag(DEFAULT_TILEBAG_FILE, LinkedList tilebag);

    void loadMosaic(DEFAULT_MOSAIC_FILE, Tile *defaultMosaicGrid);

private:
};

#endif // GAME_FILE_HANDLER
