#include <vector>
#include <fstream>
#include <string>
#include "Game.h"
#include "Types.h"
#include "GameFileHandler.h"

using std::endl;
using std::string;

GameFileHandler::GameFileHandler()
{
}

GameFileHandler::~GameFileHandler()
{
}

// void GameFileHandler::saveGame()
// {
//     //call respective methods
// }

void GameFileHandler::createFile(string fileName, Player *players[NO_OF_PLAYERS], vector<string> *turns)
{
    std::ofstream file;
    file.open(fileName);
    file << DEFAULT_TILEBAG_FILE << endl;
    int arrSize = sizeof(players) / sizeof(players[0]);
    for (int i = 0; i < arrSize; ++i)
    {
        file << players[i]->getName();
    }

    for (int i=0; i != turns->size(); i++ )
    {
        file <<  turns->at(i)  << ' ' << endl;
    }

}

void GameFileHandler::saveTurn(string file, string inputFactory, string inputTile, string inputStorageRow)
{
    std::ifstream inFile;
    inFile.open(file);

}

void GameFileHandler::loadGame()
{
}

bool GameFileHandler::loadTileBag(string file, LinkedList *tilebag)
{
    bool result = false;
    std::ifstream inFile;
    inFile.open(file);
    if (inFile.good())
    {
        string tiles;
        std::getline(inFile, tiles);
        for (unsigned int i = 0; i < tiles.length(); ++i)
        {
            tilebag->addBack(tiles.at(i));
        }
        result = true;
    }
    else
    {
        // file not found
    }
    inFile.close();
    return result;
}

bool GameFileHandler::loadMosaic(string file, Mosaic mosaic)
{
    bool result = false;
    std::ifstream inFile;
    inFile.open(file);
    if (inFile.good())
    {
        char ch;
        for (int i = 0; i < MOSAIC_DIM; ++i)
        {
            for (int j = 0; j < MOSAIC_DIM;)
            {
                inFile.get(ch);
                // accounting for newline characters
                if (ch != '\n' && ch != '\r' && ch != ' ')
                {
                    mosaic[i][j] = ch;
                    ++j;
                }
            }
        }
    }
    else
    {
        // file not found
    }
    inFile.close();
    return result;
}
