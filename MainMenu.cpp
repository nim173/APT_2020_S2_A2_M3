#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using std::cout;
using std::endl;
using std::cin;
using std::string;

void show_credits();
void show_menu();

int main(void) {
   cout << endl << "Welcome to Azul" << endl;
   cout << "---------------" << endl;
   bool loop = true;
   int input = 0;
   do {
      show_menu();
      cout << "> ";
      // if EOF character is not inputted, continue the program
      if (cin >> input) {
         if (input == 1) {
            
         } else if (input == 2) {

         } else if (input == 3) {
            cout << endl;
            show_credits();
         } else if (input == 4) {
            cout << endl;
            loop = false;
         } else if (!cin.eof()){
            cout << "Invalid input" << endl;
            // clear bad input flag
            std::cin.clear(); 
            // ignores the last input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
         }
      } else {
         cout << endl << "Goodbye" << endl << endl;
         loop = false;
      }
   } while (loop);
}

void show_credits() {
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

void show_menu() {
   cout << endl << "Menu" << endl;
   cout << "----" << endl;
   cout << "1. New Game" << endl
        << "2. Load Game" << endl
        << "3. Credits (Show student information)" << endl
        << "4. Quit" << endl << endl;
}