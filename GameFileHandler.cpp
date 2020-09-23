<<<<<<< HEAD
#include "GameFileHandler.h"

GameFileHandler::GameFileHandler()
{    
}

GameFileHandler::~GameFileHandler()
{

}

void GameFileHandler::save()
{

}

void GameFileHandler::load()
{

}

=======
#include <vector>
#include <fstream>
#include <string>
#include "Game.h"
#include "Types.h"
#include "GameFileHandler.h"

using std::string;

GameFileHandler::GameFileHandler()
{
    
}

GameFileHandler::~GameFileHandler()
{

}

void GameFileHandler::save()
{

}

void GameFileHandler::load()
{

}

bool GameFileHandler::loadTileBag(string file, LinkedList* tilebag) {
    bool result = false;
    std::ifstream inFile;
    inFile.open(file);
    if (inFile.good()) {
        string tiles;
        std::getline(inFile, tiles);
        for (int i = 0; i < tiles.length(); ++i) {
            tilebag->addBack(tiles.at(i));
        }
        result = true;
    } else {
        // file not found
    }
    inFile.close();
    return result;
}

bool GameFileHandler::loadMosaic(string file, Mosaic mosaic) {
    bool result = false;
    std::ifstream inFile;
    inFile.open(DEFAULT_MOSAIC_FILE);
    if (inFile.good()) {
        char ch;
        for (int i = 0; i < MOSAIC_DIM; ++i) {
            for (int j = 0; j < MOSAIC_DIM;) {
                inFile.get(ch);
                // accounting for newline characters
                if (ch != '\n' && ch!= '\r' && ch != ' ') {
                    mosaic[i][j] = ch;
                    ++j;
                }
            }
        }
    } else {
        // file not found
    }
    inFile.close();
    return result;
}
>>>>>>> aedcc130528e470d646fb82c12e5870018348ad0
