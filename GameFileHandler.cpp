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
    bool newGame = true;

    if (file.fail())
    {
    }

    return newGame;
}

bool GameFileHandler::saveFileCHeck(string fileName)
{
    // std::ofstream file;
    bool fileNotFound = false;

    // if (file.fail(fileName))
    // {
    //     fileNotFound = true;
    // }

    std::ifstream ifile;
    ifile.open(fileName);
    if (!ifile)
    {
        fileNotFound = true;
    }
    else
    {
        cout << "file doesn't exist";
    }
    return fileNotFound;
}

void GameFileHandler::saveGame(string fileName, LinkedList *tileBag, Player *players[NO_OF_PLAYERS], vector<string> *turns, bool newGame)
{
    std::ofstream writeFile(fileName, std::ofstream::trunc);
    std::ifstream readFile(fileName);
    writeFile.open(fileName);
    readFile.open(fileName);
    cout << "im here" << endl;

    std::ifstream initialTileBagFile;
    initialTileBagFile.open(fileName + "initial.tilebag");

    string name = fileName + "initial.tilebag";
    int size = name.length();
    char initialTileBagFileName[size];
    //turning it into a CStyle string to accomodate overwriting the file
    toCharString(name, initialTileBagFileName, size);

    //if new game then add default tile bag and save players names
    if (newGame)
    {
        writeFile << DEFAULT_TILEBAG_FILE << endl;
    }
    else
    {
        if (initialTileBagFile.good())
        {
            string sLine;
            getline(initialTileBagFile, sLine);
            writeFile << sLine << endl;
        }

        initialTileBagFile.close();

        remove(initialTileBagFileName);
    }

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

void GameFileHandler::toCharString(string fileName, char arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = fileName[i];
    }
}

void GameFileHandler::loadGame(string fileName, LinkedList *tileBag, Player *players[NO_OF_PLAYERS], vector<string> *turns)
{
    int counter = 1;
    std::ifstream readFile(fileName);
    std::ofstream tileBagFile(fileName + "initial.tilebag");
    readFile.open(fileName);
    if (readFile.good())
    {
        do
        {
            string sLine;
            getline(readFile, sLine);
            if (counter == 1)
            {
                loadTileBag(fileName, tileBag);
                //saving initial tileBag
                tileBagFile.open(fileName + "initial.tilebag");
                tileBagFile << sLine;
                tileBagFile.close();
            }
            if (counter == 2 || counter == 3)
            {
                if (counter == 2)
                {
                    players[0] = new Player(sLine);
                }
                if (counter == 3)
                {
                    players[1] = new Player(sLine);
                }
            }
            if (counter > 3)
            {
            }
        } while (!readFile.eof());
    }

    readFile.close();
}

bool GameFileHandler::loadTileBag(string file, LinkedList *tilebag)
{
    tilebag->clear();
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
