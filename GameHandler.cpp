#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <random>
#include "GameHandler.h"
#include "GameFileHandler.h"
#include "Game.h"
#include "Player.h"
#include "Types.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

GameHandler::GameHandler()
{
    // initialize file handler
    this->fileHandler = new GameFileHandler();

    tilebag = new LinkedList();

    turns = new vector<string>();

    // initialize defaultMosaic
    defaultMosaicGrid = new Tile *[MOSAIC_DIM];
    for (int i = 0; i != MOSAIC_DIM; ++i)
    {
        defaultMosaicGrid[i] = new Tile[MOSAIC_DIM];
    }
    fileHandler->loadMosaic(DEFAULT_MOSAIC_FILE, defaultMosaicGrid);
}

void GameHandler::playNewGame(bool fixedSeed, int seed)
{
    if (addPlayers())
    {
        cout << endl
             << "Let's Play!" << endl;

        fileHandler->loadTileBag(DEFAULT_TILEBAG_FILE, tilebag);
        shuffleTilebag(fixedSeed, seed);
        fileHandler->writeInitialBag(tilebag->toString());
        currentGame = new Game(tilebag);

        // play the game
        playGame(0, NO_OF_PLAYERS);

        endGame();
    } // players added successfully
    else
    {
        // EOF, handled back in main menu (where this method is called);
    }
}

void GameHandler::playGame(int startingRound, int startingPlayer)
{
    bool notEOF = true;
    int j = startingPlayer;

    for (int i = startingRound; i < NO_OF_ROUNDS && notEOF; ++i)
    {
        cout << endl
                << "== START OF ROUND " << std::to_string(i+1) << " ==" << endl;
        while (!currentGame->roundOver() && notEOF) {
            cout << endl
                 << "TURN FOR PLAYER: " << players[j % NO_OF_PLAYERS]->getName() << endl
                 << "Factories:" << endl
                 << currentGame->printFactories() << endl
                 << "Mosaic for " << players[j % NO_OF_PLAYERS]->getName() << ":" << endl
                 << players[j % NO_OF_PLAYERS]->printPlayerBoard() << endl
                 << "> ";

            int factoryNo;
            Tile tile;
            int storageRow;

            string input;
            bool result = true;
            do
            {
                factoryNo = -1;
                tile = EMPTY_SLOT;
                storageRow = -1;
                if (std::getline(cin, input)) {
                    std::stringstream stream(input);
                    result = getPlayerTurn(&stream, &factoryNo, &tile, &storageRow, true);
                    if (result) {
                        result = validateTurn(j % NO_OF_PLAYERS, factoryNo, tile, storageRow, true);
                    }
                } else {
                    notEOF = false;
                }
            } while (notEOF && !result);
            if (notEOF)
            {
                playTurn(j % NO_OF_PLAYERS, factoryNo, tile, storageRow, true);
                ++j;
            }
        }
        if (notEOF)
        {
            cout << endl
                 << "===  END OF ROUND  ===" << endl;
            cout << endl
                 << "===  ROUND RESULT  ===" << endl;
            endRound(true);

            // reset game board for all rounds except the last
            if (i != NO_OF_ROUNDS - 1)
            {
                j = resetGameBoard();
            }
        }
    }

    if (notEOF)
    {
        cout << endl
             << "====   GAME OVER  ====" << endl;
        printGameResults();
    } // EOF, handled in main menu
}

void GameHandler::endGame() {
    tilebag->clear();
    turns->clear();

    delete currentGame;
    currentGame = nullptr;

    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        delete players[i];
        players[i] = nullptr;
    }
}

void GameHandler::loadGame(string fileName, bool testing) {
    if (!fileHandler->loadFileCheck(fileName)) {
        cout << endl 
        << "File does not exist." << endl;
    }
    else {
        tilebag = new LinkedList();
        if (fileHandler->loadGame(fileName, this, tilebag, players, turns)) {
            // simulate the turns
            currentGame = new Game(tilebag);
            int round = 0;
            int j = NO_OF_PLAYERS;

            int factoryNo = -1;
            Tile tile = EMPTY_SLOT;
            int storageRow = -1; 

            bool loop = true;
            for (unsigned int i = 0; i < turns->size() && loop; ++i) {
                loop = false;
                std::stringstream stream(turns->at(i));
                if (getPlayerTurn(&stream, &factoryNo, &tile, &storageRow, false)) {
                    if (validateTurn(j % NO_OF_PLAYERS, factoryNo, tile, storageRow, false)) {
                        playTurn(j % NO_OF_PLAYERS, factoryNo, tile, storageRow, false);
                        ++j;
                        loop = true;
                    } else {
                        cout << endl << "Invalid turn format at: " << endl
                        << i+1 << ": " << turns->at(i);
                    }
                } else {
                    cout << endl << "Invalid turn with respect to game context at: " << endl
                        << i+1 << ": " << turns->at(i);
                }
                if (currentGame->roundOver()) {
                    endRound(false);
                    if (i != NO_OF_ROUNDS - 1) {
                        j = resetGameBoard();
                    }
                    ++round;
                }
            }

            if (!testing) {
                // continue game
                playGame(round, j % NO_OF_PLAYERS);

                endGame();
            }

        }
    }
}

void GameHandler::playTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame)
{
    // remove tile(s) from relevant factory, obtain number of tiles removed
    int numTilesToAdd = currentGame->removeFromFactory(factoryNo, tile);

    // handle 'f' tile if specified factory is the centre factory (0)
    // if first element of centre factory is 'F', add f to floor line of player
    if (factoryNo == 0 && currentGame->checkForFirstPlayerTile())
    {
        players[playerNo]->addToFloorLine(FIRST_PLAYER_TILE, 1);
        if (newGame)
        {
            cout << "Player " << players[playerNo]->getName() << " goes first next round" << endl;
        }
    }

    // add tile(s) to player storage row (and/or floor line)
    int num = players[playerNo]->addToStorageRow(storageRow, tile, numTilesToAdd);

    // add to tilebag if floor line of player is full
    for (int i = 0; i < num; ++i)
    {
        tilebag->addBack(tile);
    }

    if (newGame)
    {
        std::stringstream ss;
        ss << "turn ";
        ss << factoryNo;
        ss << ' ';
        ss << tile;
        ss << ' ';
        ss << storageRow;

        string turn = ss.str();
        
        turns->push_back(turn);
        cout << "Turn Successful." << endl;     
    }
}

bool GameHandler::getPlayerTurn(std::stringstream *stream, int *factoryNo, Tile *tile, int *storageRow, bool newGame)
{
    bool invalidTurn = true;
    string validTiles = VALID_TURN_TILES;
    string maxStorageRowValue = std::to_string(MAX_STORAGE_ROW_VALUE);

    string command;
    string inputStorageRow;

    string fileName = " ", choice = " ";

    *stream >> command;
    if (newGame && (command == "save" || command == "SAVE")) {
        if(*stream>>fileName){
            fileHandler->saveGame(fileName, tilebag, players, turns);
        } else {
            cout << "File name can't be empty" << endl;
        }
    } else if (command == "turn" || command == "TURN") {
        if (*stream >> *factoryNo && *factoryNo >= 0 && *factoryNo <= (NO_OF_FACTORIES - 1)) {
            *stream >> *tile;

            if (validTiles.find(*tile) != string::npos) {
                *stream >> inputStorageRow;
                invalidTurn = false;

                if (inputStorageRow == "broken"||inputStorageRow == "6"){
                    *storageRow = FLOOR_LINE_INDEX;
                } else if(inputStorageRow.compare("1") >= 0 && inputStorageRow.compare(maxStorageRowValue) <= 0) {
                    *storageRow = std::stoi(inputStorageRow);
                } else {
                    invalidTurn = true;
                    if (newGame) {
                        cout << "Invalid storage row selected, pick from 1 to " <<
                            maxStorageRowValue << " , or 'broken'" << endl;
                    }
                }
            } else {
                if (newGame) {
                    cout << "Invalid tile" << endl;
                }
            }
        } else {
            if (newGame) {
                cout << "Out of range for factory, pick from 0 to " << (NO_OF_FACTORIES - 1) << endl;
            }
        }
    } else {
        if (newGame) {
            cout << "Invalid command, use the following format" << endl
                    << "turn <factory> <tile-code> <storage row>" << endl;
        }
    }

    if (invalidTurn && newGame) {   
        cout << endl << "Enter turn again" << endl << "> ";
    }

    return !invalidTurn;
}

bool GameHandler::validateTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame)
{
    string errorMessage = "Invalid turn: ";
    bool result = (currentGame->validateTurn(factoryNo, tile, &errorMessage) &&
                   players[playerNo]->validateTurn(tile, storageRow, &errorMessage));
    if (!result && newGame)
    {
        cout << errorMessage << endl
             << "Enter turn again" << endl
             << "> ";
    }
    return result;
}

bool GameHandler::addPlayers()
{
    for (int i = 0; i < NO_OF_PLAYERS; ++i)
    {
        if (players[i] != nullptr)
        {
            delete players[i];
        }
    }

    bool result = false;

    cout << endl
         << "Starting a New Game" << endl
         << endl;
    cout << "Enter a name for player 1" << endl;
    string player1Name;
    cout << "> ";
    if (std::getline(cin, player1Name))
    {
        cout << endl
             << "Enter a name for player 2" << endl
             << "> ";
        string player2Name;
        do
        {
            if (std::getline(cin, player2Name))
            {
                if (player1Name != player2Name)
                {
                    players[0] = new Player(player1Name);
                    players[1] = new Player(player2Name);
                    result = true;
                }
                else
                {
                    cout << "Error: Players cannot have the same name" << endl
                         << endl
                         << "Enter a name for player 2" << endl
                         << "> ";
                }
            } // if not EOF
        } while (!cin.eof() && player1Name == player2Name);
    } // if not EOF

    // returns true if both players added successfully
    // returns false if EOF
    return result;
}

void GameHandler::endRound(bool newGame)
{
    if (newGame)
    {
        for (int i = 0; i < NO_OF_PLAYERS; ++i)
        {
            cout << "Mosaic for Player " << players[i]->getName() << ":" << endl
                 << players[i]->printPlayerBoard() << endl;
        }
        cout << endl
             << "Points Scored:" << endl;
    }
    for (int i = 0; i < NO_OF_PLAYERS; ++i)
    {
        int points = players[i]->updateScore(defaultMosaicGrid, tilebag);
        if (newGame)
        {
            cout << "Player " + players[i]->getName() + ": " << std::to_string(points) << endl
                 << players[i]->printPlayerBoard() << endl;
        }
    }
    if (newGame)
    {
        printPlayerPoints("Total Points");
    }
}

void GameHandler::printGameResults()
{
    printPlayerPoints("Final Scores:");

    int max = 0;
    bool drawn = false;
    for (int i = 1; i < NO_OF_PLAYERS; ++i)
    {
        if (players[i]->getPoints() > players[max]->getPoints())
        {
            max = i;
            drawn = false;
        }
        else if (players[i]->getPoints() == players[max]->getPoints())
        {
            drawn = true;
        }
    }

    string message;
    if (drawn == false)
    {
        message = "Player " + players[max]->getName() + " wins!";
    }
    else
    {
        message = "Game drawn!";
    }
    cout << message << endl;
}

void GameHandler::printPlayerPoints(string message)
{
    cout << endl
         << message << endl;
    for (int i = 0; i < NO_OF_PLAYERS; ++i)
    {
        cout << "Player " + players[i]->getName() + ": " + std::to_string(players[i]->getPoints()) << endl;
    }
}

int GameHandler::resetGameBoard()
{
    int result = -1;
    for (int i = 0; i < NO_OF_PLAYERS; ++i)
    {
        if (players[i]->resetFloorline(tilebag))
        {
            result = i;
        }
    }
    currentGame->populateFactories(tilebag);
    return result;
}

void GameHandler::testGame(string fileName) {
    loadGame(fileName, true);
    //print factories
    //check if load is successful
    if ((turns->size() > 0)) {
        cout << endl
            << endl
            << "Factories:" << endl
            << currentGame->printFactories() << endl;

        //print boards.
        for (int i = 0; i < NO_OF_PLAYERS; i++) {
            players[i]->printPlayerBoard();

            cout << endl
                << "Score for player " << players[i]->getName() << ": " << players[i]->getPoints();

            cout << endl
                << "Mosaic for " << players[i]->getName() << ":" << endl
                << players[i]->printPlayerBoard() << endl;
        }
    }
    else {
        cout << "Load Failed." << endl;
    }   

    endGame();
}

void GameHandler::shuffleTilebag(bool fixedSeed, int seed) {
    std::random_device engine; 
    std::default_random_engine fixedSeedEngine(seed);
    int randValue = 0;
    for (int i = tilebag->getSize(); i > 1; --i) {
        std::uniform_int_distribution<int> uniform_dist(1, i);
        if (fixedSeed != true) {
            randValue = uniform_dist(engine);
        } else {
            randValue = uniform_dist(fixedSeedEngine);
        }
        if (i != randValue) {
            tilebag->swap(i, randValue);
        }
    }
}
