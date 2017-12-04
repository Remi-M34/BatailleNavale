#include "../include/headers.h"
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
Jeu Jeu(6,1,2,6);



Jeu.start();


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
  Window erreur(10,COLS,1,1);
  erreur.print(0,1,erreurTailleFenetre());
cin.ignore(1);
assert(COLS > 140 && LINES > 40);
}
  Menu menu;

  menu.mainMenu(1);

  // myprogram();
  // stopProgramX();

  return 0;
}
