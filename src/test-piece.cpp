#include "../include/window.h"
#include "../include/config.h"
#include "../include/menu.h"
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

int main()
{
  startProgramX();
  if (COLS < 140 || LINES < 40)
  {
    erreurEcran(140,40);
  }




  Menu menu;

  menu.mainMenu(1);

  // myprogram();
  // stopProgramX();

  return 0;
}

