#include "navire.h"
#include "window.h"
#include "config.h"
#include "menu.h"
#include "message.h"
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include "string"
using namespace std;

#define xm (COLS-100)/2
#define ym (LINES-33)/2


void menu()
{

  startProgramX();

  Window plateau(33, 100, xm, ym, WMAGENTA);
  plateau.setBordureDroite();

  // bordure.setBordureDroite();

            Window ap(2, 40, 50 - 40 / 2+xm, 25+ym, ' ');
            ap.setCouleurBordure(WBLACK);


  Color col[5] = {WMAGENTA, BMAGENTA, BMAGENTA, BMAGENTA, BMAGENTA};

  refresh();
  getch();

  int c;
  string choix[5] = {"Commencer une partie", "Options", "Aide","Version du jeu", "A Propos"};
  int selection = 0;


  Flotte flotte(50 - getDimFlotte('w')+xm, 4+ym,1);

  bool wait = false;

  while ((c = getch()) != 'q')
  {


    for (int i = 0; i < 5 && !wait; i++)
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
      if (selection != 4)
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
        stopProgramX();
  return;      }
      else if (selection == 1)
      {
        options();
        menu();
        return;
        // version.print(2, 2, "aaaaaaaaaaaaaaa", WGREEN);
        // refresh();
      }
      else if (selection == 2)
      {
        aideMenu();
        menu();
        return;
      }
      else if (selection == 3)
      {
                ap.clear();
          ap.print(0,1,version());
      }
      else if (selection == 4)
      {
        ap.clear();
          ap.print(0,1,aPropos());
          ap.setBordureDroite();
          
      }
      else{
        changerDim();
        menu();
        return;
      }
      break;
    }
  }

  erase();

  stopProgramX();

}










// Menu  des options

void options()
{
  stopProgramX();
  startProgramX();

  Window plateau(33, 100, xm, ym, WMAGENTA);
  plateau.setBordureDroite();

  Color col[3] = {WMAGENTA, BMAGENTA, BMAGENTA};

  refresh();
  getch();

  int c;
  string choix[3] = {"Modifier les navires", "Modifier la taille de la grille","Retour"};
  int selection = 0;


  Flotte flotte(50 - getDimFlotte('w')+xm, 4+ym,0);

    for (int i = 0; i < 3 ; i++)
    {
      plateau.print(50 - choix[i].length() / 2, 12 + 2 * i, choix[i], col[i]);
      usleep(12000);
    }


  while ((c = getch()) != 'q')
  {

    for (int i = 0; i < 3 ; i++)
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
      if (selection != 2)
      {
        swap(col[selection], col[selection + 1]);
        selection++;
      }

      break;
    case '\n':
      if (selection == 0)
      {
        menuNavire();
        options();
        return;
      }
      else if (selection == 1)
      {
                changerDim();

        options();
        return;
        // version.print(2, 2, "aaaaaaaaaaaaaaa", WGREEN);
        // refresh();
      }
      else if (selection == 2)
      {
        return;
        // version.print(2, 2, "aaaaaaaaaaaaaaa", WGREEN);
        // refresh();
      }

      break;
    }
  }

  erase();

  stopProgramX();

}




void aideMenu()
{
  int ch;
  Window aidef(19,85,7+xm,12+ym);
  aidef.setBordureDroite();
  aidef.setCouleurFenetre(WBLACK);
            aidef.print(39, 0, "Règles :", WBLUE);

          aidef.print(0, 2, aide(), WBLACK);
                  aidef.print(39, 18, "Retour", WMAGENTA);

    while ((ch = getch()) != '\n')
    {

    }

}