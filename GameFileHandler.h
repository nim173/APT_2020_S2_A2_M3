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

void save();
void load();


private:

};


#endif // GAME_FILE_HANDLER