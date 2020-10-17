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
#include "AI_Turn.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

GameHandler::GameHandler()
{
    // initialize file handler
    this->fileHandler = new GameFileHandler();

    tilebag = new LinkedList();

    boxLid = new LinkedList();

    turns = new vector<string>();

    // initialize defaultMosaic
    defaultMosaicGrid = new Tile *[MOSAIC_DIM];
    for (int i = 0; i != MOSAIC_DIM; ++i)
    {
        defaultMosaicGrid[i] = new Tile[MOSAIC_DIM];
    }
    fileHandler->loadMosaic(DEFAULT_MOSAIC_FILE, defaultMosaicGrid);
}

GameHandler::~GameHandler() {
    if (currentGame != nullptr) {
        delete currentGame;
        currentGame = nullptr;
    }

    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        if (players[i] != nullptr) {
            delete players[i];
            players[i] = nullptr;
        }
    }

    if (tilebag != nullptr) {
        delete tilebag;
        tilebag = nullptr;
    }
    if (boxLid != nullptr) {
        delete boxLid;
        boxLid = nullptr;
    }
    if (turns != nullptr) {
        delete turns;
        turns = nullptr;
    }
    if (fileHandler != nullptr) {
        delete fileHandler;
        fileHandler = nullptr;
    }

    if (defaultMosaicGrid != nullptr) {
        // delete defaultMosaicGrid
        for (int i = 0; i != MOSAIC_DIM; ++i) {
            delete[] defaultMosaicGrid[i];
        }
        delete[] defaultMosaicGrid;
    }
}

void GameHandler::playNewGame(bool fixedSeed, int seed, bool advancedMode, bool AImode) {
    if (addPlayers(advancedMode, AImode)) {
        if (fileHandler->loadTileBag(DEFAULT_TILEBAG_FILE, tilebag)) {
            cout << endl
                 << "Let's Play!" << endl;
            if (advancedMode) {
                for (int i = 0; i < NUM_TILES_PER_COLOUR; ++i) {
                    tilebag->addBack(ADV_MODE_SIXTH_TILE);
                }
            } // add 20 extra tiles of 'O' for the advanced mode
            shuffleTilebag(fixedSeed, seed);
            fileHandler->writeInitialBag(tilebag->toString());
            currentGame = new Game(tilebag, boxLid);

            // play the game
            playGame(0, NO_OF_PLAYERS, advancedMode, AImode);

            endGame();
        } else {
            cout << endl << "Error In File: " << DEFAULT_TILEBAG_FILE << endl;
        }
    } // players added successfully
    else {
        // EOF, handled back in main menu (where this method is called);
    }
}

void GameHandler::playGame(int startingRound, int startingPlayer, bool advancedMode, bool AImode) {
    bool notEOF = true;
    int j = startingPlayer;

    for (int i = startingRound; i < NO_OF_ROUNDS && notEOF; ++i) {
        cout << endl
             << "== START OF ROUND " << std::to_string(i+1) << " ==" << endl;
        while (!currentGame->roundOver() && notEOF) {
            cout << tilebag->toString() << endl;
            cout << boxLid->toString() << endl;
            cout << endl << "TURN FOR ";
            if (!players[j % NO_OF_PLAYERS]->isCpu()) {
                cout << "PLAYER: " << players[j % NO_OF_PLAYERS]->getName() << endl;
            } else {
                cout << "CPU:" << endl;
            }
            cout << "Factories:" << endl
                << currentGame->printFactories() << endl
                << "Mosaic for " ;
            if (!players[j % NO_OF_PLAYERS]->isCpu()) {
                cout << "Player " << players[j % NO_OF_PLAYERS]->getName() << ":" << endl;
            } else {
                cout << "CPU:" << endl;
            }
            cout << players[j % NO_OF_PLAYERS]->printPlayerBoard() << endl
            << "> ";

            int factoryNo;
            Tile tile;
            int storageRow;

            if (!players[j % NO_OF_PLAYERS]->isCpu()) {
                string input;
                bool result = true;
                do {
                    factoryNo = -1;
                    tile = EMPTY_SLOT;
                    storageRow = -1;
                    if (std::getline(cin, input)) {
                        std::stringstream stream(input);
                        result = getPlayerTurn(&stream, &factoryNo, &tile, &storageRow, true, advancedMode, AImode);
                        if (result) {
                            result = validateTurn(j % NO_OF_PLAYERS, factoryNo, tile, storageRow, true);
                        }
                    } else {
                        notEOF = false;
                    }
                } while (notEOF && !result);
            } else {
                getAIturn(&factoryNo, &tile, &storageRow);
            }
            if (notEOF) {
                playTurn(j % NO_OF_PLAYERS, factoryNo, tile, storageRow, true);
                ++j;
            }
        }
        if (notEOF) {
            cout << endl
                 << "===  END OF ROUND  ===" << endl;
            if (advancedMode) {
                notEOF = advancedModeWallTiling();
            }
            if (notEOF) {
                cout << endl
                     << "===  ROUND RESULT  ===" << endl;
            
                endRound(true, advancedMode);
            }
            
            // reset game board for all rounds except the last
            if (i != NO_OF_ROUNDS - 1 && notEOF)
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
    boxLid->clear();
    turns->clear();

    if (currentGame != nullptr) {
        delete currentGame;
        currentGame = nullptr;
    }
}

void GameHandler::loadGame(string fileName, bool testing) {
    if (!fileHandler->loadFileCheck(fileName)) {
        cout << endl 
        << "File does not exist." << endl;
    }
    else {
        bool advancedMode = false;
        bool AIMode = false;
        if (fileHandler->loadGame(fileName, this, tilebag, players, turns, &advancedMode, &AIMode)) {
            // simulate the turns
            currentGame = new Game(tilebag, boxLid);
            int round = 0;
            int j = NO_OF_PLAYERS;

            int factoryNo = -1;
            Tile tile = EMPTY_SLOT;
            int storageRow = -1; 

            bool loop = true;
            for (unsigned int i = 0; i < turns->size() && loop; ++i) {
                loop = false;
                std::stringstream stream(turns->at(i));
                if (getPlayerTurn(&stream, &factoryNo, &tile, &storageRow, false, advancedMode, AIMode)) {
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
                    if (advancedMode) {
                        // simulate wall tiling
                        for (int k = 0; k < NO_OF_PLAYERS; ++k) {
                            int row = 0;
                            for (row = 0; row < ADV_MOSAIC_DIM && loop; ++row) {
                                if (players[k]->isFilled(row)) {
                                    string input;
                                    int wallCol;
                                    loop = false;
                                    wallCol = -1;
                                    std::stringstream stream(turns->at(++i));
                                    if (getPlayerWallTilePlacement(&stream, &wallCol, true)) {
                                        Tile tile = EMPTY_SLOT;
                                        if (players[k]->placeTileInWall(row, wallCol-1, &tile)) {
                                            loop = true;
                                            int points = players[k]->getPointsForAdjacentTiles(boxLid, row, wallCol-1, ADV_MOSAIC_DIM, tile, false);
                                            players[k]->setPoints(players[k]->getPoints() + points);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (loop) {
                        endRound(false, advancedMode);
                        if (i != NO_OF_ROUNDS - 1) {
                            j = resetGameBoard();
                        }
                        ++round;
                    }
                }
            }

            if (!testing && loop) {
                // continue game
                playGame(round, j % NO_OF_PLAYERS, advancedMode, AIMode);

                endGame();
            }

        }
    }
}

void GameHandler::playTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame) {
    // remove tile(s) from relevant factory, obtain number of tiles removed
    int numTilesToAdd = currentGame->removeFromFactory(factoryNo, tile);

    // handle 'f' tile if specified factory is the centre factory (0)
    // if first element of centre factory is 'F', add f to floor line of player
    if (factoryNo == 0 && currentGame->checkForFirstPlayerTile(false)) {
        players[playerNo]->addToFloorLine(FIRST_PLAYER_TILE, 1);
        if (newGame) {
            if (!players[playerNo]->isCpu()) {
                cout << "Player " << players[playerNo]->getName();
            } else {
                cout << "CPU";
            }
            cout << " goes first next round" << endl;
        }
    }

    // add tile(s) to player storage row (and/or floor line)
    int num = players[playerNo]->addToStorageRow(storageRow, tile, numTilesToAdd);

    // add to lid box if floor line of player is full
    for (int i = 0; i < num; ++i) {
        boxLid->addBack(tile);
    }

    if (newGame) {
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

bool GameHandler::getPlayerTurn(std::stringstream *stream, int *factoryNo, Tile *tile, 
    int *storageRow, bool newGame, bool advancedMode, bool AImode)
{
    string validTiles = VALID_TURN_TILES;
    string maxStorageRowValue = "";
    if (!advancedMode) {
        maxStorageRowValue = std::to_string(MOSAIC_DIM+1);
    } else {
        maxStorageRowValue = std::to_string(ADV_MOSAIC_DIM+1);
    }

    string command;
    string inputStorageRow;
    string fileName = " ", choice = " ";
    
    bool invalidTurn = true;
    *stream >> command;
    if (newGame && (command == "save" || command == "SAVE")) {
        if(*stream>>fileName){
            fileHandler->saveGame(fileName, tilebag, players, turns, advancedMode, AImode);
        } else {
            cout << "File name can't be empty" << endl;
        }
    } else if (command == "turn" || command == "TURN") {
        if (*stream >> *factoryNo && *factoryNo >= 0 && *factoryNo <= (NO_OF_FACTORIES - 1)) {
            *stream >> *tile;

            if (validTiles.find(*tile) != string::npos) {
                *stream >> inputStorageRow;
                invalidTurn = false;
                if (inputStorageRow == "broken" || inputStorageRow == "BROKEN"){
                    *storageRow = std::stoi(maxStorageRowValue);
                } else if(inputStorageRow.compare(std::to_string(MIN_STORAGE_ROW)) >= 0 
                            && inputStorageRow.compare(maxStorageRowValue) <= 0) {
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

bool GameHandler::validateTurn(int playerNo, int factoryNo, Tile tile, int storageRow, bool newGame) {
    string errorMessage = "Invalid turn: ";
    bool result = (currentGame->validateTurn(factoryNo, tile, &errorMessage) &&
                   players[playerNo]->validateTurn(tile, storageRow, &errorMessage));
    if (!result && newGame) {
        cout << errorMessage << endl
             << "Enter turn again" << endl
             << "> ";
    }
    return result;
}

bool GameHandler::addPlayers(bool advancedMode, bool AImode) {
    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        if (players[i] != nullptr) {
            delete players[i];
            players[i] = nullptr;
        }
    }

    bool result = false;

    cout << endl
         << "Starting a New Game" << endl
         << endl;
    cout << "Enter a name for player 1" << endl;
    string player1Name;
    cout << "> ";
    do {
        if (std::getline(cin, player1Name)) {
            if (player1Name.find_first_not_of(' ') != std::string::npos) {
                if (!AImode) {
                    cout << endl
                        << "Enter a name for player 2" << endl
                        << "> ";
                    string player2Name;
                    do {
                        if (std::getline(cin, player2Name)) {
                            if (player1Name != player2Name && (player2Name.find_first_not_of(' ') != std::string::npos)) {
                                players[0] = new Player(player1Name, advancedMode);
                                players[1] = new Player(player2Name, advancedMode);
                                result = true;
                            } else {
                                cout << "Error: Players cannot have the same name or be empty" << endl
                                    << endl
                                    << "Enter another name for player 2" << endl
                                    << "> ";
                            } // if players have same name or player 2 name is empty
                        } // if not EOF
                    } while (!cin.eof() && !result);
                } else {
                    players[0] = new Player(player1Name, advancedMode);
                    players[1] = new Player("", advancedMode);
                    players[1]->setCpu(true);
                    result = true;
                }
            } else {
                cout << "Error: Player name cannot be empty" << endl
                     << endl
                     << "Enter another name for player 1" << endl
                     << "> ";
            }
        } // if not EOF
    } while (!cin.eof() && !result);

    // returns true if both players added successfully
    // returns false if EOF
    return result;
}

void GameHandler::endRound(bool newGame, bool advancedMode) {
    if (newGame && !advancedMode) {
        for (int i = 0; i < NO_OF_PLAYERS; ++i) {
            cout << "Mosaic for ";
            if (!players[i]->isCpu()) {
                cout << "Player " << players[i]->getName() << ":" << endl;
            } else {
                cout << "CPU :" <<  endl;
            }
            cout << players[i]->printPlayerBoard() << endl;
        }
        cout << endl
            << "Points Scored:" << endl;
    }
    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        int points = players[i]->updateScore(defaultMosaicGrid, boxLid, advancedMode);
        if (newGame && !advancedMode) {
            if (!players[i]->isCpu()) {
                cout << "Player " + players[i]->getName() + ": " << std::to_string(points) << endl;
            } else {
                cout << "CPU : " << std::to_string(points) << endl;
            }
        }
        if (newGame) {
            cout << players[i]->printPlayerBoard() << endl;
        }
    }
    if (newGame) {
        printPlayerPoints("Total Points");
    }
}

bool GameHandler::advancedModeWallTiling() {
    bool notEOF = true;

    cout << "Use the following command for wall tiling - 'place <wall column index(1-" 
                << ADV_MOSAIC_DIM << ")>" << endl;

    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        int row = 0;
        for (row = 0; row < ADV_MOSAIC_DIM && notEOF; ++row) {
            if (players[i]->isFilled(row)) {
                cout << endl << "Mosaic for Player " << players[i]->getName() << ":" << endl
                    << players[i]->printPlayerBoard() << endl
                    << "For row " << (row+1) << endl
                    << "> ";

                string input;
                bool result = true;
                int wallCol;
                do {
                    wallCol = -1;
                    if (std::getline(cin, input)) {
                        std::stringstream stream(input);
                        result = getPlayerWallTilePlacement(&stream, &wallCol, true);
                        Tile tile = EMPTY_SLOT;
                        if (result) {
                            result = players[i]->placeTileInWall(row, wallCol-1, &tile);
                            if (!result) {
                                cout << "Column " << wallCol << " already filled in row " << row+1 << endl;
                            }
                        }
                        if (!result) {
                            cout << endl << "Enter command again" << endl << "> ";
                        } else {
                            // add move to turns
                            turns->push_back("place " + std::to_string(wallCol));
                            // add points gained from move to player
                            int points = players[i]->getPointsForAdjacentTiles(boxLid, row, wallCol-1, ADV_MOSAIC_DIM, tile, false);
                            players[i]->setPoints(players[i]->getPoints() + points);
                            cout << "Points scored for move: " << points << endl;
                        }
                    } else {
                        notEOF = false;
                    }
                } while (notEOF && !result);
            }
        }
    }
    return notEOF;
}

bool GameHandler::getPlayerWallTilePlacement(std::stringstream *stream, int *wallCol, bool newGame) {
    bool invalidTurn = true;
    string command;
    *stream >> command;
    if (command == "place" || command == "PLACE") {
        if (*stream >> *wallCol && *wallCol >= 1 && *wallCol <= ADV_MOSAIC_DIM) {
            invalidTurn = false;
        } else {
            cout << "Out of range for wall, pick from 1 to " << ADV_MOSAIC_DIM << endl;
        }
    } else {
        cout << "Invalid command, use the following format" << endl
             << "place <wall column index(1-" << ADV_MOSAIC_DIM << ")>" << endl;
    }
    return !invalidTurn;
}

void GameHandler::printGameResults() {
    printPlayerPoints("Final Scores:");

    int max = 0;
    bool drawn = false;
    for (int i = 1; i < NO_OF_PLAYERS; ++i) {
        if (players[i]->getPoints() > players[max]->getPoints()) {
            max = i;
            drawn = false;
        } else if (players[i]->getPoints() == players[max]->getPoints()) {
            drawn = true;
        }
    }

    string message;
    if (drawn == false) {
        if (!players[max]->isCpu()) {
            message = "Player " + players[max]->getName();
        } else {
            message = "CPU ";
        }
        message += " wins!";
    } else {
        message = "Game drawn!";
    }
    cout << message << endl;
}

void GameHandler::printPlayerPoints(string message) {
    cout << endl
         << message << endl;
    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        if (!players[i]->isCpu()) {
            cout << "Player " << players[i]->getName() << ": ";
        } else {
            cout << "CPU : ";
        }
        cout << std::to_string(players[i]->getPoints()) << endl;
    }
}

int GameHandler::resetGameBoard()
{
    int result = -1;
    for (int i = 0; i < NO_OF_PLAYERS; ++i)
    {
        if (players[i]->resetFloorline(boxLid))
        {
            result = i;
        }
    }
    currentGame->populateFactories(tilebag, boxLid);
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
            cout << endl << "Score for ";
            if (!players[i]->isCpu()) {
                cout << "player " << players[i]->getName() << ": ";
            } else {
                cout << "CPU : ";
            }
            cout << players[i]->getPoints();

            cout << endl
                << "Mosaic for ";
            if (!players[i]->isCpu()) {
                cout << players[i]->getName() << ":" << endl;
            } else {
                cout << "CPU:" << endl;
            }
            cout << players[i]->printPlayerBoard() << endl;
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

void GameHandler::getAIturn(int *factoryNo, Tile *tile, int *storageRow) {
    // obtaining all valid turns
    vector<AI_Turn> *moves = new vector<AI_Turn>;
    int chosenIndex = 0;
    vector<AI_Turn> *movesToFloorLine = new vector<AI_Turn>;
    int numTiles = 0;
    int points = 0;
    Tile temp = EMPTY_SLOT;
    for (int i = 0; i < NO_OF_FACTORIES; ++i) {
        string validTiles = VALID_TURN_TILES;
        // last tile ('O') not considered for AI mode
        for (unsigned int j = 0; j < validTiles.length()-1; ++j) {
            temp = validTiles.at(j);
            // get number of tiles of current type in factory
            numTiles = currentGame->getNumberOfTiles(i, temp);
            if (numTiles > 0) {
                int excess = 0;
                if (i == 0 && currentGame->checkForFirstPlayerTile(true)) {
                    ++excess;
                } // if the turns results in addition of first-player-tile ('F') to floor line
                for (int k = MIN_STORAGE_ROW; k <= MOSAIC_DIM; ++k) {
                    if (validateTurn(CPU_PLAYER_NO, i, temp, k, false)) {
                        points = players[CPU_PLAYER_NO]->getPointsForTurn(defaultMosaicGrid, temp, k-1, 
                            numTiles, &excess);
                        moves->push_back(AI_Turn(i, temp, k, numTiles, points, excess));
                    }
                }
                if (i == 0 && currentGame->checkForFirstPlayerTile(true)) {
                    excess = 1;
                }
                points = players[CPU_PLAYER_NO]->getPointsForTurn(defaultMosaicGrid, temp, FLOOR_LINE_INDEX, 
                            numTiles, &excess);
                movesToFloorLine->push_back(AI_Turn(i, temp, FLOOR_LINE_INDEX+1, numTiles, points, excess));
            } // if there are specified tiles in the factory
        }
    }
    cout << "Moves : " << moves->size() << endl;
    cout << "MovesF : " << movesToFloorLine->size() << endl;

    if (moves->size() == 0) {
        // only valid moves left are moves to floor line
        // pick move with maximum points to floor line
        int maxIndex = 0;
        for (unsigned int i = 0; i < movesToFloorLine->size(); ++i) {
            if (movesToFloorLine->at(i).getPoints() > movesToFloorLine->at(maxIndex).getPoints()) {
                maxIndex = i;
                cout << "max " << maxIndex << endl;
            }
        }
        *factoryNo = movesToFloorLine->at(maxIndex).getFactoryNo();
        *tile = movesToFloorLine->at(maxIndex).getTile();
        *storageRow = movesToFloorLine->at(maxIndex).getStorageRow();
    } else {
        if (players[CPU_PLAYER_NO]->isStorageRowsEmpty()) {
            // pick the move with the highest number of points with highest num of tiles and lowest excess tiles
            int maxPointsIndex = 0;
            for (unsigned int i = 0; i < moves->size(); ++i) {
                if (moves->at(i).getPoints() > moves->at(maxPointsIndex).getPoints()) {
                    maxPointsIndex = i;
                } else if (moves->at(i).getPoints() == moves->at(maxPointsIndex).getPoints()) {
                    if (moves->at(i).getNumTiles() > moves->at(maxPointsIndex).getNumTiles()){
                        maxPointsIndex = i;
                    } else if (moves->at(i).getNumTiles() == moves->at(maxPointsIndex).getNumTiles()){
                        if (moves->at(i).getExcessTiles() < moves->at(maxPointsIndex).getExcessTiles()){
                            maxPointsIndex = i;
                        }
                    }
                }
            }

            if (moves->at(maxPointsIndex).getPoints() == 0 && moves->at(maxPointsIndex).getExcessTiles() == 0) {
                // if no points can be taken from the move, pick the maximum tiles with smallest fit
                int diff = moves->at(maxPointsIndex).getStorageRow() - moves->at(maxPointsIndex).getNumTiles();
                int minDiffIndex = maxPointsIndex;
                for (unsigned int i = maxPointsIndex+1; i < moves->size(); ++i) {
                    if (moves->at(i).getPoints() == moves->at(maxPointsIndex).getPoints() && moves->at(i).getExcessTiles() == 0) {
                        int tempDiff = moves->at(i).getStorageRow() - moves->at(i).getNumTiles();
                        if (tempDiff < diff) {
                            diff = tempDiff;
                            minDiffIndex = i;
                        }
                    }
                }
                maxPointsIndex = minDiffIndex;
            }

            chosenIndex = maxPointsIndex;
        } // if no partially filled rows present 
        else {
            // pick the move with the highest number of points with highest num of tiles and lowest excess tiles
            int maxPointsIndex = 0;
            for (unsigned int i = 0; i < moves->size(); ++i) {
                if (moves->at(i).getPoints() > moves->at(maxPointsIndex).getPoints()) {
                    maxPointsIndex = i;
                } else if (moves->at(i).getPoints() == moves->at(maxPointsIndex).getPoints()) {
                    if (moves->at(i).getNumTiles() > moves->at(maxPointsIndex).getNumTiles()){
                        maxPointsIndex = i;
                    } else if (moves->at(i).getNumTiles() == moves->at(maxPointsIndex).getNumTiles()){
                        if (moves->at(i).getExcessTiles() < moves->at(maxPointsIndex).getExcessTiles()){
                            maxPointsIndex = i;
                        }
                    }
                }
            }

            cout << "Max: " << maxPointsIndex << endl;
            cout << moves->at(maxPointsIndex).getPoints() << moves->at(maxPointsIndex).getFactoryNo() << moves->at(maxPointsIndex).getTile() << moves->at(maxPointsIndex).getStorageRow()<< moves->at(maxPointsIndex).getNumTiles()<<endl;

            // if points == 0, look for smallest fit
            // while giving priority for partially filled storage rows
            if (moves->at(maxPointsIndex).getPoints() == 0 && moves->at(maxPointsIndex).getExcessTiles() == 0) {
                int numTilesinRow = players[CPU_PLAYER_NO]->numTilesInRow(moves->at(maxPointsIndex).getStorageRow()-1);
                int diff = moves->at(maxPointsIndex).getStorageRow()-numTilesinRow - moves->at(maxPointsIndex).getNumTiles();
                int minDiffIndex = maxPointsIndex;
                for (unsigned int i = maxPointsIndex+1; i < moves->size(); ++i) {
                    if (moves->at(i).getPoints() == moves->at(maxPointsIndex).getPoints() && moves->at(i).getExcessTiles() == 0) {
                        int tempNumTilesinRow = players[CPU_PLAYER_NO]->numTilesInRow(moves->at(i).getStorageRow()-1);
                        int tempDiff = moves->at(i).getStorageRow()-numTilesinRow - moves->at(i).getNumTiles();
                        if (tempDiff < diff) {
                            diff = tempDiff;
                            minDiffIndex = i;
                            numTilesinRow = tempNumTilesinRow;
                        } else if (tempDiff == diff) {
                            if (tempNumTilesinRow > 0 && numTilesinRow == 0) {
                                diff = tempDiff;
                                minDiffIndex = i;
                                numTilesinRow = tempNumTilesinRow;
                            }
                        }
                    }
                }
                maxPointsIndex = minDiffIndex;
            }

           chosenIndex = maxPointsIndex;
        } // if partially filled rows present
        
        if (moves->at(chosenIndex).getPoints() >= 0) {
            *factoryNo = moves->at(chosenIndex).getFactoryNo();
            *tile = moves->at(chosenIndex).getTile();
            *storageRow = moves->at(chosenIndex).getStorageRow();
        } else {
            // if chosen turn is <0, check in floorline moves if there is a better move that could be made
            int maxIndex = -1;
            int maxPoints = moves->at(chosenIndex).getPoints();
            for (unsigned int i = 0; i < movesToFloorLine->size(); ++i) {
                if (movesToFloorLine->at(i).getPoints() > maxPoints) {
                    maxIndex = i;
                    maxPoints = movesToFloorLine->at(i).getPoints();
                }
            }

            if (maxIndex != -1) {
                *factoryNo = movesToFloorLine->at(maxIndex).getFactoryNo();
                *tile = movesToFloorLine->at(maxIndex).getTile();
                *storageRow = movesToFloorLine->at(maxIndex).getStorageRow();
            }
        }

        // delete turns
        delete moves;
        delete movesToFloorLine;
    }

    cout << "Turn for CPU: turn " << *factoryNo << " " << *tile << " " << *storageRow << endl; 
}
