#include "window.h"
#include "config.h"
#include "grille.h"
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;









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

  menu.print(0,0,"Placez vos navires sur la grille \n Entrée    Sélectionner / placer \n Espace    Pivoter \n b         Renvoyer le navire au port",WBLACK);
    joueur.selectionNavire() ;


  while ((ch = getch()) != 'q')
  {
  }


}


















void menu()
{

  startProgramX();


  Window plateau(33, 100, 0, 0, WMAGENTA);
  plateau.setBordureDroite();
















  Color col[4] = {WMAGENTA, BMAGENTA, BMAGENTA, BMAGENTA};

  refresh();
  getch();

  int c;
  string choix[4] = {"Commencer une partie", "Version du jeu", "Aide", "A Propos"};
  int selection = 0;


        Window version(3,27,50-27/2,25,' ');
        version.clear();



Flotte flotte(50-getDimFlotte('w'),5);


  while ((c = getch()) != 'q')
  {

    for (int i = 0; i < 4; i++)
    {
      plateau.print(50 - choix[i].length() / 2, 12 + 2 * i, choix[i], col[i]);
    }
    switch (c)
    {
    case KEY_UP:
      if (selection != 0)
      {
        swap(col[selection], col[selection - 1]);
        selection--;
      }
      break;

    case KEY_DOWN:
      if (selection != 3)
      {
        swap(col[selection], col[selection + 1]);
        selection++;
      }


      break;
    case '\n':
      if (selection == 0)
      {
                erase();
  plateau.setCouleurBordure(BYELLOW);

        myprogram();
      }
      else if (selection == 1)
      {
        version.print(2,2,"aaaaaaaaaaaaaaa",WGREEN);
        refresh();
      }
      break;
    }
  }

  erase();

  stopProgramX();

  usleep(999999);
}














int main()
{

  menu();

  startProgramX();
  myprogram();
  stopProgramX();
  return 0;
}
