#include <iostream>
#include <string>
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

bool GameHandler::playTurn(int playerNo, Game* game) {
    cout << "TURN FOR PLAYER: " << players[playerNo]->getName() << endl
         << "Factories:" << endl 
         << game->printFactories() << endl
         << endl 
         << "Mosaic for " + players[playerNo]->getName() << endl
         << players[playerNo]->printPlayerBoard() << endl
         << "> ";

    string factory;
    string tile;
    string storageRow;

    getPlayerTurn(&factory, &tile, &storageRow);
    // remove tile(s) from relevant factory, obtain number of tiles removed - Game.removeFromFactory(int factory, char tile)
    // add tile(s) to player storage row - Player.addToStorageRow(int row, char tile) & Player.addBrokenTiles(char tile)

    // handle 'f' tile 
    // if first element of centre factory is f, add f to broken tile vector of player - Player.addBrokenTiles('f')
    return false;
}

void getPlayerTurn(string* factory, string* tile, string* storageRow) {
    string maxFactoryValue = std::to_string(NO_OF_FACTORIES - 1);
    string validTiles = VALID_TURN_TILES;
    string maxStorageRowValue = std::to_string(MOSAIC_DIM);

    string command;

    bool invalidTurn = true;

    while (invalidTurn) {
        if (cin >> command) {
            if (command == "turn") {
                if (cin >> *factory) {
                    if (factory->compare("0") >= 0 && factory->compare(maxFactoryValue) <= 0) {
                        if (cin >> *tile) {
                            if (validTiles.find(*tile) != string::npos) {
                                if (cin >> *storageRow) {
                                    if (storageRow->compare("1") >= 0 && storageRow->compare(maxStorageRowValue) <= 0) {
                                        invalidTurn = false;
                                        // cout << command << " " << *factory << " " << *tile << " " << *storageRow << endl; 
                                    } else {
                                        // not a valid storage row number (should be bet 1 & maxStorageRowValue)
                                        cout << *storageRow << ": invalid storage row number, pick from 1 to " 
                                                           << maxStorageRowValue << endl;
                                    }
                                } else {
                                    // EOF
                                }
                            } else {
                                // tile is not a valid tile
                                cout << *tile + ": is not a valid tile" <<endl;
                            }
                        } else {
                            // EOF
                        }
                        
                    } // in range for factory
                    else {
                        cout << *factory << ": out of range for factory, pick from 0 to " << maxFactoryValue <<endl;
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