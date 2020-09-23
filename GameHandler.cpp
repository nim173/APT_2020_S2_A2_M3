#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include "GameHandler.h"
#include "GameFileHandler.h"
#include "Game.h"
#include "Player.h"
#include "Types.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

GameHandler::GameHandler() {
    // initialize file handler
    this->fileHandler = new GameFileHandler();

    // initialize default tilebag
    tilebag = new LinkedList();
    fileHandler->loadTileBag(DEFAULT_TILEBAG_FILE, tilebag); // TODO - ERROR CHECKING
    
    // initialize defaultMosaic 
    defaultMosaicGrid = new Tile*[MOSAIC_DIM];
    for (int i = 0; i != MOSAIC_DIM; ++i) {
         defaultMosaicGrid[i] = new Tile[MOSAIC_DIM];
    }
    fileHandler->loadMosaic(DEFAULT_MOSAIC_FILE, defaultMosaicGrid); // TODO - ERROR CHECKING

    // initialize turns
    turns = new vector<string>();
}

void GameHandler::playNewGame() {
    if (addPlayers()) {
        cout << endl << "Let's Play!" << endl;
        Game* newGame = new Game(tilebag);

        // play the game
        bool eof = false;
        int j = NO_OF_PLAYERS;
        for (int i = 0; i < NO_OF_ROUNDS && !eof; ++i) {
            // TODO - player with f starts first
            cout << endl << "=== START OF ROUND ===" << endl;
            // while (!newGame->roundOver() && !eof) {
            //     eof = playTurn(j % NO_OF_PLAYERS, newGame);
            //     ++j;
            // }
            if (!eof) {
                cout << endl << "===  END OF ROUND  ===" << endl;
                cout << endl << "===  ROUND RESULT  ===" << endl;
                endRound();

                // reset game board for all rounds except the last
                if (i != NO_OF_ROUNDS - 1) {
                    j = resetGameBoard(newGame);
                }
            }
        }

        if (!eof) {
            cout << endl << "====   GAME OVER  ====" << endl;
            // Scoring - TODO
            // printGameResults();
        } // EOF, handled in main menu
        delete newGame;
    } // players added successfully
    else {
        // EOF, handled back in main menu (where this method is called);
    }
}

bool GameHandler::loadGame() {
    return false;
}

bool GameHandler::playTurn(int playerNo, Game* game) {
    cout << "TURN FOR PLAYER: " << players[playerNo]->getName() << endl
         << "Factories:" << endl 
         << game->printFactories() << endl
         << endl 
         << "Mosaic for " + players[playerNo]->getName() << endl
         << players[playerNo]->printPlayerBoard() << endl
         << "> ";

    int factoryNo;
    Tile tile;
    int storageRow;

    bool result = true;
    do {
        result = getPlayerTurn(&factoryNo, &tile, &storageRow);
    } while (result && !validateTurn(playerNo, game, factoryNo, tile, storageRow));

    if (result) {
        // remove tile(s) from relevant factory, obtain number of tiles removed
        // int numTilesToAdd = game->removeFromFactory(factoryNo, tile);

        // add tile(s) to player storage row (and/or floor line)
        // players[playerNo]->addToStorageRow(storageRow, tile, numTilesToAdd);

        // handle 'f' tile if specified factory is the centre factory (0)
        // if first element of centre factory is 'F', add f to floor line of player
        if (factoryNo == 0 && game->checkForFirstPlayerTile()) {
            players[playerNo]->addToFloorLine((Tile)FIRST_PLAYER_TILE, 1);
        }
    } // not EOF
    return result;
}

bool GameHandler::getPlayerTurn(int* factoryNo, Tile* tile, int* storageRow) {
    string inputFactory;
    string inputTile;
    string inputStorageRow;

    string maxFactoryValue = std::to_string(NO_OF_FACTORIES - 1);
    string validTiles = VALID_TURN_TILES;
    string maxStorageRowValue = std::to_string(MOSAIC_DIM);

    string command;

    bool result = true;
    bool invalidTurn = result;

    while (invalidTurn) {
        if (cin >> command) {
            if (command == "turn") {
                if (cin >> inputFactory) {
                    if (inputFactory.compare("0") >= 0 && inputFactory.compare(maxFactoryValue) <= 0) {
                        *factoryNo = std::stoi(inputFactory);
                        if (cin >> inputTile) {
                            if (validTiles.find(inputTile) != string::npos) {
                                *tile = (Tile)inputTile.at(0);
                                if (cin >> inputStorageRow) {
                                    if (inputStorageRow.compare("1") >= 0 && inputStorageRow.compare(maxStorageRowValue) <= 0) {
                                        *storageRow = std::stoi(inputStorageRow);
                                        invalidTurn = false;
                                        // cout << command << " " << *factory << " " << *tile << " " << *storageRow << endl; 
                                    } else {
                                        // not a valid storage row number (should be bet 1 & maxStorageRowValue)
                                        cout << inputStorageRow << ": invalid storage row number, pick from 1 to " 
                                                           << maxStorageRowValue << endl;
                                    }
                                } else {
                                    // EOF
                                }
                            } else {
                                // tile is not a valid tile
                                cout << inputTile + ": is not a valid tile" <<endl;
                            }
                        } else {
                            // EOF
                        }
                        
                    } // in range for factory
                    else {
                        cout << inputFactory << ": out of range for factory, pick from 0 to " << maxFactoryValue <<endl;
                    } 
                } else {
                    // EOF
                }
            } else {
                // invalid command - turn <factory> <tile-code> <storage row>
                cout << command << ": invalid command, use the following format" << endl 
                     << "turn <factory> <tile-code> <storage row>" << endl;
            }
        } else {
            // EOF
        }

        if (invalidTurn) {
            if (cin.eof()) {
                cout << endl << "EOF" <<endl;
                invalidTurn = false;
                result = false;
            } else {
                cout << endl << "Enter turn again" << endl
                    << "> ";
                cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
    return result;
}

bool GameHandler::validateTurn(int playerNo, Game* game, int factoryNo, Tile tile, int storageRow) {
    string errorMessage = "Invalid turn: ";
    bool result (game->validateTurn(factoryNo, tile, &errorMessage) && 
            players[playerNo]->validateTurn(tile, storageRow, &errorMessage));
    cout << errorMessage << endl 
                << "Enter turn again" << endl
                << "> ";
    return result;
}

// TODO - handle players having same name
bool GameHandler::addPlayers() {
    bool result = false;
    
    cout << endl << "Starting a New Game" << endl << endl;
    cout << "Enter a name for player 1" << endl;
    string player1Name;
    cout << "> ";
    if (cin >> player1Name) {
        cout << endl << "Enter a name for player 2" << endl << "> ";
        string player2Name;
        do {
            if (cin >> player2Name) {
                if (player1Name != player2Name) {
                    players[0] = new Player(player1Name);
                    players[1] = new Player(player2Name);
                    result = true;
                } else {
                    cout << "Error: Players cannot have the same name" << endl << endl
                        << "Enter a name for player 2" << endl << "> ";
                }
            } // if not EOF
        } while (!cin.eof() && player1Name == player2Name);
    } // if not EOF
    
    // returns true if both players added successfully
    // returns false if EOF
    return result;
}

void GameHandler::endRound() {
    cout << endl << "Points Scored" << endl;
    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        cout << "Player " + players[i]->getName() + ": " 
                + std::to_string(players[i]->updateScore(defaultMosaicGrid, tilebag)) << endl;
        
    }
    printPlayerPoints("Total Points");
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
        message = "Player " + players[max]->getName() + " wins!";
    } else {
        message = "Game drawn!";
    }
    cout << message << endl;
}

void GameHandler::printPlayerPoints(string message) {
    cout << endl << message << endl;
    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        cout << "Player " + players[i]->getName() + ": " 
                + std::to_string(players[i]->getPoints()) << endl;
    }
}

int GameHandler::resetGameBoard(Game* game) {
    int result = -1;
    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        if (players[i]->resetFloorline(tilebag)) {
            result = i;
        }
    }
    game->populateFactories(tilebag);
    return result;
}
