#include "window.h"
#include "config.h"
#include "grille.h"
#include "message.h"
#include "jeu.h"
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

#define H getHeightGrille()
#define W getWidthGrille()
#define WF getDimFlotte('w')
#define WH getDimFlotte('h')

void myprogram()
{
  clear();

  // Window menu(4, max(2 * getWidthGrille(), WF * 2 - 4), 6, 1);
  // menu.setCouleurBordure(WBLACK);
  // menu.setCouleurFenetre(WBLACK);
  // menu.setBordureDroite();
  int sxScore = 6 + 2 * W;
  int syScore = 3;
  int scoreWidth = 18;
  int scoreHeight = 4 + H;
  int sy = 5;
  int sy2 = 5;
  int syf = 10 + H;
  int syf2 = syf;
  int sx2, sx, sxf, sxf2;

  if (W < WF)
  {
    sxf = 2;
    sx = (WF - W) + 2;
    sxScore += WF - W;
    sxf2 = scoreWidth +8 + sx + W * 2 - (WF-W);
  }
  else
  {
    sx = 2;
    sxf = 2 + (W - WF);
    sxf2 = scoreWidth +8 + sx + W * 2 + (W-WF);
  }
    int bordureWidth = (sxScore + (scoreWidth/2))*2;
    int bordureHeight = H+WH+12;

  sx2 = scoreWidth + 8 + sx + W * 2;

// Pour centrer tout le jeu
  int xm = (COLS - bordureWidth) /2;
  int ym = (LINES - bordureHeight)/2;

sx += xm;
sy +=ym;
sxf +=xm;
syf +=ym;
sx2 += xm;
sy2 +=ym;
sxf2 +=xm;
syf2 +=ym;
sxScore += xm;
syScore += ym;

  Window bordure(bordureHeight, bordureWidth, xm, ym);
  bordure.setBordureDroite();

  refresh();

  Window score(scoreHeight, scoreWidth, sxScore, syScore);
  score.setCouleurBordure(WBLUE);

  int ch;
  int const h = 10;
  int const w = 20;

noecho();
Jeu Jeu2(sx, sy, sxf, syf, sx2, sy2, sxf2, syf2);

  // jeu.joueur1.flotte.initSelection();


  while (1)
  {
refresh();
  }
  // Création des grilles et flottes dont les positions dépendent des dimensions calculées dans le fichier config
  // Grille joueur(6, 9, 6, 12 + getHeightGrille());

  // Grille joueur[2] = Grille joueur(5,5,3,3);

  // Grille ennemi(10 + 2 * max(WF, getWidthGrille()), 9, 10 + 2 * max(WF, getWidthGrille()),  12 + getHeightGrille());

  // joueur.flotte.initSelection();

  // menu.print(0, 0, insPlacement(), WBLACK);
  // joueur.selectionNavire();

  // while ((ch = getch()) != 'q')
  // {
  // }
}

// Grille* creationGrilles(int n, int j)
// {

//   for (int i = 0 ; i < n ; i++)
//   {
//     if (j > 0)
//     {
//       Grille Grilles i(6+20*i, 9, 6, 12 + getHeightGrille());

//     }
//   }
//   return Grille;
// }

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
