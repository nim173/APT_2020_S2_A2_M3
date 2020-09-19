#include <iostream>
#include <string>
#include <limits>
#include "GameHandler.h"
#include "Game.h"
#include "Player.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

void GameHandler::playNewGame() {
    if (addPlayers()) {
        cout << endl << "Let's Play!" << endl;
        Game* newGame = new Game();

        // play the game
        for (int i = 0; i < NO_OF_ROUNDS; ++i) {
            // TODO - player with f starts first
            cout << endl << "=== START OF ROUND ===" << endl;
            // int j = NO_OF_PLAYERS;
            // while (!newGame->roundOver()) {
            //     playTurn(j % NO_OF_PLAYERS, &newGame);
            //     ++j;
            // }
            cout << endl << "===  END OF ROUND  ===" << endl;
            printRoundResults();
        }

        cout << endl << "====   GAME OVER  ====" << endl;
        // Scoring - TODO
        // printGameResults();

        delete newGame;
    } // players added successfully
    else {
        // EOF, handled back in main menu (where this method is called);
    }
}

bool GameHandler::loadGame() {
    return false;
}

void GameHandler::playTurn(int playerNo, Game* game) {
    cout << "TURN FOR PLAYER: " << players[playerNo]->getName() << endl
         << "Factories:" << endl 
         << game->printFactories() << endl
         << endl 
         << "Mosaic for " + players[playerNo]->getName() << endl
         << players[playerNo]->printPlayerBoard() << endl
         << "> ";

    int factoryNo;
    char tile;
    int storageRow;

    do {
        getPlayerTurn(&factoryNo, &tile, &storageRow);
    } while (!validateTurn(playerNo, game, factoryNo, tile, storageRow));

    // remove tile(s) from relevant factory, obtain number of tiles removed
    // int numTilesToAdd = game->removeFromFactory(factoryNo, tile);

    // add tile(s) to player storage row (and/or floor line)
    // players[playerNo]->addToStorageRow(storageRow, tile, numTilesToAdd);

    // handle 'f' tile if specified factory is the centre factory (0)
    // if first element of centre factory is 'F', add f to floor line of player
    if (factoryNo == 0 && game->checkForFirstPlayerTile()) {
        players[playerNo]->addToFloorLine(FIRST_PLAYER_TILE, 1);
    }
}

void GameHandler::getPlayerTurn(int* factoryNo, char* tile, int* storageRow) {
    string inputFactory;
    string inputTile;
    string inputStorageRow;

    string maxFactoryValue = std::to_string(NO_OF_FACTORIES - 1);
    string validTiles = VALID_TURN_TILES;
    string maxStorageRowValue = std::to_string(MOSAIC_DIM);

    string command;

    bool invalidTurn = true;

    while (invalidTurn) {
        if (cin >> command) {
            if (command == "turn") {
                if (cin >> inputFactory) {
                    if (inputFactory.compare("0") >= 0 && inputFactory.compare(maxFactoryValue) <= 0) {
                        *factoryNo = std::stoi(inputFactory);
                        if (cin >> inputTile) {
                            if (validTiles.find(inputTile) != string::npos) {
                                *tile = inputTile.at(0);
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
            cout << endl << "Enter turn again" << endl
                 << "> ";
            cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

bool GameHandler::validateTurn(int playerNo, Game* game, int factoryNo, char tile, int storageRow) {
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
        cout << endl << "Enter a name for player 2" << endl;
        string player2Name;
        cout << "> ";
        if (cin >> player2Name) {
            players[0] = new Player(player1Name);
            players[1] = new Player(player2Name);
            result = true;
        } // if not EOF
    } // if not EOF
    
    // returns true if both players added successfully
    // returns false if EOF
    return result;
}

void GameHandler::printRoundResults() {
    // TODO after figuring out scoring from player
}

void GameHandler::printGameResults() {
    cout << endl << "Final Scores:" << endl;

    for (int i = 0; i < NO_OF_PLAYERS; ++i) {
        cout << "Player " + players[i]->getName() + ": " 
                + std::to_string(players[i]->getPoints()) << endl;
    }
}
