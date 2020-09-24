#include <vector>
#include <fstream>
#include <string>
#include "Game.h"
#include "Types.h"
#include "GameFileHandler.h"

using std::endl;
using std::cout;
using std::cin;
using std::string;

GameFileHandler::GameFileHandler()
{}

GameFileHandler::~GameFileHandler()
{}

bool checkForFile()
{

}

void GameFileHandler::saveGame(string fileName, Player *players[NO_OF_PLAYERS], vector<string> *turns, bool newGame)
{
    std::ofstream file;
    file.open(fileName);
    char choice = ' ';

    if (newGame==true)
    {
        file << DEFAULT_TILEBAG_FILE << endl;

        for (int i = 0; i < NO_OF_PLAYERS; ++i)
        {
            file << players[i]->getName();
        }
    }

    for (int i = 0; i != turns->size(); i++)
    {
        file << turns->at(i) << endl;
    }

    file.close();
}

void GameFileHandler::loadGame(string fileName)
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
