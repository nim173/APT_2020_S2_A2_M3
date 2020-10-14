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

void GameFileHandler::saveGame(string fileName, LinkedList *tileBag, Player *players[NO_OF_PLAYERS], vector<string> *turns)
{
    //open/create the save file.
    std::ofstream saveFile;
    saveFile.open ("saveFiles/"+fileName);

    //first load the initial tileBag
    std::ifstream initialTileBagFile (INITIAL_TILEBAG_FILE);
    string initialTileBag; 
    initialTileBagFile >> initialTileBag;
    saveFile << initialTileBag <<endl;
    initialTileBagFile.close();

    //save player names
    for(int i = 0; i < NO_OF_PLAYERS; i++){
        saveFile << players[i]->getName() << endl;
    }
    //save all turns
    for(unsigned int i = 0; i < turns->size(); i++){
        saveFile << turns->at(i) << endl;
    }
    //close the save file. 
    saveFile.close();

    cout << "Save Successful!" << endl;
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
    std::stringstream ss;
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
                ss << ch;
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

        writeInitialBag(ss.str());

        // read player names
        if (valid) {
            for (int i = 0; i < NO_OF_PLAYERS && valid; ++i) {
                if (std::getline(readFile, line)) {
                    // remove /r from windows files
                    if (!line.empty() && line[line.size() - 1] == '\r') {
                        line.erase(line.size() - 1);
                    }
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
    std::stringstream ss;
    bool result = false;
    std::ifstream inFile;
    inFile.open(file);
    if (inFile.good())
    {
        string tiles;
        std::getline(inFile, tiles);
        string validTiles = VALID_TURN_TILES;
        for (unsigned int i = 0; i < tiles.length(); ++i)
        {
            if (validTiles.find(tiles.at(i)) != string::npos) {
                tilebag->addBack(tiles.at(i));
                ss << tiles.at(i);
            }
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

void GameFileHandler::writeInitialBag(string intialBag){
    std::ofstream initialBAG;
    initialBAG.open (INITIAL_TILEBAG_FILE);
    initialBAG << intialBag;
    initialBAG.close();
}

