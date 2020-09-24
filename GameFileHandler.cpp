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
{
}

GameFileHandler::~GameFileHandler()
{
}

void GameFileHandler::saveGame(string fileName, Player *players[NO_OF_PLAYERS], vector<string> *turns)
{
    std::ofstream file;
    file.open(fileName);
    int arrSize = sizeof(players) / sizeof(players[0]);
    bool newGame = true;
    char choice = ' ';

   do
   {
       if (file.fail())
       {
           cout << "File does not exist. Would you like to start a new Game?[Y/N]" << endl;
           cin >> choice;
           if (tolower(choice) == 'y')
           newGame = true;
           if (tolower(choice == 'n'))
           {
               cout << "Enter the filename from which load a game" << endl;
               cin >> fileName;
           }
       }

   }while ((file.fail() && !newGame) || !file.fail());

    if (file.fail())
    {
        file << DEFAULT_TILEBAG_FILE << endl;

        for (int i = 0; i < arrSize; ++i)
        {
            file << players[i]->getName();
        }
    }

    for (int i = 0; i != turns->size(); i++)
    {
        file << turns->at(i) << ' ' << endl;
    }
}

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

    for (int i = 0; i != turns->size(); i++)
    {
        file << turns->at(i) << ' ' << endl;
    }
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
