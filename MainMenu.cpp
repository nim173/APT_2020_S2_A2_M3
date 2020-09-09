#include <iostream>

using std::cout;
using std::endl;

void show_credits();
void show_menu();

int main(void) {
   show_menu(); 
   bool loop = true;
   do {
      int input;
      std::cin >> input;
      if (input == 3) {
         show_credits();
      } else if (input == 4) {
         loop = false;
      }

   } while (loop);
}

void show_credits() {
}

void show_menu() {
   cout << "Welcome to Azul" << endl;
   cout << "---------------" << endl
        << endl;
   cout << "Menu" << endl;
   cout << "----" << endl;
   cout << "1. New Game" << endl
        << "2. Load Game" << endl
        << "3. Credits (Show student information)" << endl
        << "4. Quit" << endl;
}