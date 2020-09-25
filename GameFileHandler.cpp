#include <vector>
#include <fstream>
#include <string>
#include "Game.h"
#include "Types.h"
#include "GameFileHandler.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

GameFileHandler::GameFileHandler()
{
}

GameFileHandler::~GameFileHandler()
{
}

bool GameFileHandler::loadFileCheck(string fileName)
{
    std::ofstream file;
    file.open(fileName);
    bool newGame = true, returnToMenu = false;
    char choice = ' ';

    do
    {
        if (file.fail())
        {
            cout << "File does not exist. Would you like to try again?[Y/N]" << endl;
            cin >> choice;
            if (tolower(choice) == 'y')
                newGame = true;
            if (tolower(choice) == 'n')
                returnToMenu = true;
        }

    } while (((file.fail() && !newGame) || !file.is_open()) && !returnToMenu);

    return newGame;
}

bool GameFileHandler::saveFileCHeck(string fileName)
{
    std::ofstream file;
    file.open(fileName);
    bool newGame = true, fileNotFound = false;
    char choice = ' ';

    if (file.fail())
    {
        fileNotFound = true;
    }
  return fileNotFound;

}

void GameFileHandler::saveGame(string fileName, LinkedList tileBag, Player *players[NO_OF_PLAYERS], vector<string> *turns)
{
    std::ofstream writeFile(fileName, std::ofstream::trunc);
    std::ifstream readFile(fileName);
    writeFile.open(fileName);
    readFile.open(fileName);
    char choice = ' ';

    //if new game then add default tile bag and save players names
    writeFile << DEFAULT_TILEBAG_FILE << endl;

    for (int i = 0; i < NO_OF_PLAYERS; ++i)
    {
        writeFile << players[i]->getName() << std::endl;
    }

    for (vector<string>::const_iterator i = (*turns).begin(); i != (*turns).end(); ++i)
    {
        writeFile << *i << std::endl;
    }

    for (unsigned int i = 0; i != turns->size(); i++)
    {
        writeFile << turns->at(i) << endl;
    }

    writeFile.close();
    readFile.close();
}

void GameFileHandler::loadGame(string fileName)
{
}

bool GameFileHandler::loadTileBag(string file, LinkedList *tilebag) {
    bool result = false;
    std::ifstream inFile;
    inFile.open(file);
    if (inFile.good()) {
        string tiles;
        std::getline(inFile, tiles);
        for (unsigned int i = 0; i < tiles.length(); ++i) {
            tilebag->addBack(tiles.at(i));
        }
        result = true;
    }
    else {
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
