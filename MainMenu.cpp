#include <iostream>
#include <fstream>
#include <string>
#include "Game.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

void show_menu();
void start_new_game();
void load_game();
void show_credits();

int main(void) {
   cout << endl << "Welcome to Azul" << endl;
   cout << "---------------" << endl;
   bool loop = true;
   string input;
   do {
      show_menu();
      cout << "> ";
      if (cin >> input) {
         if (input == "1") {
            start_new_game();
         } else if (input == "2") {
            load_game();
         } else if (input == "3") {
            show_credits();
         } else if (input == "4") {
            cout << endl;
            loop = false;
         } else {
            cout << "Invalid input" << endl;
         }
      } // if not EOF
      if (cin.eof()) {
         cout << endl << "Goodbye" << endl << endl;
         loop = false;
      } // if EOF
   } while (loop);
}

void show_menu() {
   cout << endl << "Menu" << endl;
   cout << "----" << endl;
   cout << "1. New Game" << endl
        << "2. Load Game" << endl
        << "3. Credits (Show student information)" << endl
        << "4. Quit" << endl << endl;
}

void start_new_game() {
   cout << "Starting a New Game" << endl << endl;
   cout << "Enter a name for player 1" << endl;
   string player1Name;
   cout << "> ";
   if (cin >> player1Name) {
      cout << endl << "Enter a name for player 2" << endl;
      string player2Name;
      cout << "> ";
      if (cin >> player2Name) {
         Game* newGame = new Game(player1Name, player2Name);
         // play the game
         // ...
         delete newGame;
      } // if not EOF
   } // if not EOF
}

void load_game() {
   
}

void show_credits() {
   cout << endl;
   string line;
   std::ifstream myFile("credits.txt");
   if (myFile.is_open())
   {
      while (getline(myFile, line))
      {
         cout << line << endl;
      }
   }
   myFile.close();
}