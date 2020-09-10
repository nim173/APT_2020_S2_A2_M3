#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::string;

void show_credits();
void show_menu();

int main(void)
{

   bool loop = true;
   do
   {
      int input=0;
      show_menu();
      std::cout << "> ";
      std::cin >> input;
      if (input == 3)
      {
         show_credits();
      }
      else if (input == 4)
      {
         loop = false;
      }

   } while (loop);
}

void show_credits()
{
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

void show_menu()
{
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