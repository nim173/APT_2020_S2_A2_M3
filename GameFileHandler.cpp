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
         Player *players[NO_OF_PLAYERS], vector<string> *turns, bool* advancedMode) {
    bool valid = true;
    *advancedMode = false;
    std::ifstream readFile;
    std::ofstream tileBagFile;
    readFile.open(fileName);
    string line;
    string validTiles = VALID_TURN_TILES;
    if (readFile.good()) {
        if (std::getline(readFile, line)) {
            if (line == NEW_SAVED_GAME_FORMAT) {
                if (std::getline(readFile, line)) {
                    if (line == ADV_SAVED_GAME) {
                        *advancedMode = true;
                        if (!std::getline(readFile, line)) {
                            valid = false;
                        } else {    
                            // in advanced mode, continue to read tilebag
                        }
                    } else {
                        // expected to read "advancedMode=on"
                        valid = false;
                    }
                } else {
                    valid = false;
                }
            } else {
                // not in advanced mode, continue to read tilebag
            }
        } else {
            valid = false;
        }
        if (!valid) {
            cout << endl <<"Error: In Saved Game file" << endl;
        } else {
            // read tilebag
            valid = addToTileBag(line, tileBag);

            if (valid && tileBag->getSize() < TILEBAG_MIN_SIZE) {
                cout << endl << "Error: Tilebag is too small, should be atleast 100" << endl;
                valid = false;
            }
        }

        // read player names
        if (valid) {
            writeInitialBag(tileBag->toString());
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
                    players[i] = new Player(line, advancedMode);
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
        result = addToTileBag(tiles, tilebag);
    }
    else
    {
        // file not found
    }
    inFile.close();
    return result;
}

bool GameFileHandler::addToTileBag(string tiles, LinkedList *tilebag) {
    bool result = true;
    string validTiles = VALID_TURN_TILES;
    for (unsigned int i = 0; i < tiles.length() && result; ++i)
    {
        if (validTiles.find(tiles.at(i)) != string::npos) {
            tilebag->addBack(tiles.at(i));
        } else {
            if (tiles.at(i) != '\r' || ' ') {
                result = false;
            }
        }
    }
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

