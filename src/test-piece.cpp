#include "../include/window.h"
#include "../include/config.h"
#include "../include/menu.h"
#include "../include/message.h"

#include <iostream>

using namespace std;
void options();

#define H getHeightGrille()
#define W getWidthGrille()
#define WF getDimFlotte('w')
#define WH getDimFlotte('h')

void myprogram()
{
  clear();

  refresh();

  int ch;
  int const h = 10;
  int const w = 20;

  noecho();
  // Jeu Jeu(6, 1, 2, 6);

  // Jeu.start();

  while (1)
  {
    refresh();
  }
}

void ParametreLancement(char *argv)
{
  string par = argv;
  if (par == "version")
  {
    cout << version() << endl;
  }
  else if (par == "auteurs")
  {
    cout << aPropos() << endl;
    ;
  }
  else if (par == "aide")
  {
    cout << aide() << endl;
    ;
  }
  else
  {
    cout << "Commande inconnue.\nParamètres possibles : aide version auteurs" << endl;
  }
}

int main(int argc, char *argv[])
{

  if (argc > 1)
  {
    ParametreLancement(argv[1]);
  }
  else
  {
    startProgramX();
    if (COLS < 140 || LINES < 40)
    {
      erreurEcran(140, 40);
    }
    Menu menu;

    menu.mainMenu(1);
  }

  // myprogram();
  // stopProgramX();

  return 0;
}
