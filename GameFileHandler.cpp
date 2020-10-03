#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "Game.h"
#include "Types.h"
#include "GameFileHandler.h"
#include "GameHandler.h"

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
    std::ifstream file;
    file.open(fileName);
    bool result = false;

    if (file.good())
    {
        result = true;
    }

    return result;
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
    ifile.close();
    return fileNotFound;
}

void GameFileHandler::saveGame(string fileName, LinkedList *tileBag, Player *players[NO_OF_PLAYERS], vector<string> *turns, bool newGame)
{
    // std::ofstream writeFile(fileName, std::ofstream::trunc);
    // std::ifstream readFile(fileName);
    // writeFile.open(fileName);
    // readFile.open(fileName);
    // cout << "im here" << endl;

    // std::ifstream initialTileBagFile;
    // initialTileBagFile.open(fileName + "initial.tilebag");

    // string name = fileName + "initial.tilebag";
    // int size = name.length();
    // char initialTileBagFileName[size];
    // //turning it into a CStyle string to accomodate overwriting the file
    // toCharString(name, initialTileBagFileName, size);

    // //if new game then add default tile bag and save players names
    // if (newGame)
    // {
    //     writeFile << DEFAULT_TILEBAG_FILE << endl;
    // }
    // else
    // {
    //     if (initialTileBagFile.good())
    //     {
    //         string sLine;
    //         getline(initialTileBagFile, sLine);
    //         writeFile << sLine << endl;
    //     }

    //     initialTileBagFile.close();

    //     remove(initialTileBagFileName);
    // }

    // for (int i = 0; i < NO_OF_PLAYERS; ++i)
    // {
    //     writeFile << players[i]->getName() << std::endl;
    // }

    // for (vector<string>::const_iterator i = (*turns).begin(); i != (*turns).end(); ++i)
    // {
    //     writeFile << *i << std::endl;
    // }

    // for (unsigned int i = 0; i != turns->size(); i++)
    // {
    //     writeFile << turns->at(i) << endl;
    // }

    // writeFile.close();
    // readFile.close();    

    std::ofstream saveFile;
    std::ifstream initialTileBagFile (DEFAULT_TILEBAG_FILE);
    

    saveFile.open ("saveFiles/"+fileName);
    string initialTileBag; 
    initialTileBagFile >> initialTileBag;
    saveFile << initialTileBag <<endl;
    initialTileBagFile.close();


    for(int i = 0; i < NO_OF_PLAYERS; i++){
        saveFile << players[i]->getName() << endl;
    }
    for(unsigned int i = 0; i < turns->size(); i++){
        saveFile << turns->at(i) << endl;
    }


    saveFile.close();

    
}

void GameFileHandler::toCharString(string fileName, char arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = fileName[i];
    }
}

bool GameFileHandler::loadGame(string fileName, GameHandler* gameHandler, LinkedList *tileBag,
         Player *players[NO_OF_PLAYERS], vector<string> *turns)
{
    bool valid = true;;
    std::ifstream readFile;
    std::ofstream tileBagFile;
    readFile.open(fileName);
    string line;
    char ch;
    string validTiles = VALID_TURN_TILES;
    if (readFile.good()) {
        // read tilebag
        readFile.get(ch);
        while (ch != '\n' && valid && !readFile.eof()) {
            if (validTiles.find(ch) != string::npos) {
                tileBag->addBack(ch);
            } else {
                if (ch != '\r') {
                    cout << endl <<"Error: Invalid characters found in tilebag" << endl;
                    valid = false;
                }
            }
            readFile.get(ch);
        }

        if (valid && tileBag->getSize() < TILEBAG_MIN_SIZE) {
            cout << endl << "Error: Tilebag is too small, should be atleast 100" << endl;
            valid = false;
        }

        // read player names
        if (valid) {
            for (int i = 0; i < NO_OF_PLAYERS && valid; ++i) {
                if (std::getline(readFile, line)) {
                    if (players[i] != nullptr) {
                        delete players[i];
                        players[i] = nullptr;
                    }
                    players[i] = new Player(line);
                } else {
                    cout << endl <<"Error: Can't read player " << i+1 << " name" << endl;
                    valid = false;
                }
            }
        }

        // read turns
        if(valid) {
            while (!readFile.eof()) {
                if (std::getline(readFile, line)) {
                    turns->push_back(line);
                }
            }
        }
    }

    readFile.close();
    return valid;
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

