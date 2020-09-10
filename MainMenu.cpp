#include <iostream>
#include <fstream>
#include <string>

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
   do {
      int input=0;
      show_menu();
      cout << "> ";
      cin >> input;
      cout << endl;
      if (input == 3) {
         show_credits();
      }
      else if (input == 4) {
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