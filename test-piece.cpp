#include "window.h"
#include "config.h"
#include "grille.h"
#include "message.h"
#include "menu.h"
#include "navire.h"
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;
void options();



void myprogram()
{
  Window bordure(33, 100, 0, 0, ' ');
  bordure.setCouleurBordure(WGREEN);
  refresh();
  Window menu(4, max(2 * getWidthGrille(), getDimFlotte('w') * 2 - 4), 6, 1);
  menu.setCouleurBordure(WBLACK);
  menu.setCouleurFenetre(WBLACK);
  menu.setBordureDroite();

  int ch;
  int const h = 10;
  int const w = 20;

  Window score(3, max(2 * getWidthGrille(), (getDimFlotte('w') * 2) - 4), 2 * max(7 + getDimFlotte('w'), 5 + getWidthGrille()), 13 + getHeightGrille());

  score.setCouleurBordure(BGREEN);

  // Création des grilles et flottes dont les positions dépendent des dimensions calculées dans le fichier config
  Grille joueur(6, 9, 6, 12 + getHeightGrille());

  Grille ennemi(10 + 2 * max(getDimFlotte('w'), getWidthGrille()), 9, 10 + 2 * max(getDimFlotte('w'), getWidthGrille()), 1);

  joueur.flotte.initSelection();

  menu.print(0, 0, insPlacement(), WBLACK);
  joueur.selectionNavire();

  while ((ch = getch()) != 'q')
  {
  }
}


























int main()
{


  //     fstream config("config.txt", ios::in | ios::out | ios::trunc);
  //     config << "15x15\nxxxxx\n\nx\nx\nx\nx\nx\n\nx\n\nxx\nxx\n\nxxx\n x";


  // config.close();




  // int **n = new int *[3];

  // for (int i = 0; i < 3; i++)
  // {
  //   n[i] = new int[4];
  //   n[i][0] = 1;
  //   n[i][1] = 1;
  //   n[i][2] = 1;
  //   n[i][3] = 1;
  // }

  // modifierNavires(2, 3, 4, n);

  menu();

  startProgramX();
  myprogram();
  stopProgramX();

  return 0;
}
