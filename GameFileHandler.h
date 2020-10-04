#ifndef GAME_FILE_HANDLER
#define GAME_FILE_HANDLER

#include <string>
#include "Types.h"

using std::string;

class GameHandler;

class GameFileHandler
{
public:
    GameFileHandler();
    ~GameFileHandler();

    bool loadFileCheck(string fileName);

    void saveGame(string fileName, LinkedList *tileBag, Player *players[NO_OF_PLAYERS], vector<string> *turns);

    bool loadGame(string fileName, GameHandler* gameHandler, LinkedList *tileBag, Player *players[NO_OF_PLAYERS], vector<string> *turns);

    bool loadTileBag(string file, LinkedList* tilebag);

    bool loadMosaic(string file, Mosaic defaultMosaicGrid);

    void toCharString(string fileName, char arr[], int size);


private:
};

#endif // GAME_FILE_HANDLER
