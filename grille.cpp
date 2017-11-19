#include "grille.h"
#include "config.h"
#include "window.h"
#include "flotte.h"
#include "config.h"
#include <unistd.h>
using namespace std;

Grille::Grille(int const sx, int const sy, int sxf, int syf) : fenetre(getHeightGrille(), getWidthGrille() * 2, sx, sy), flotte(sxf, syf)
{

      //Initialise la grille et rend toutes les cases de la grille VIDE
      init();
}

Grille::~Grille() {}

void Grille::selectionNavire()
{
      int n = flotte.getPremierNavire();
      int ch;

      while ((ch = getch()) != 'q')

      {
            switch (ch)
            {
            case KEY_RIGHT:
                  if (n != 4)
                  {
                        flotte.echangeSelection(n, flotte.getNavireSuivant(n));
                        n = flotte.getNavireSuivant(n);
                        flotte.refreshPort(0);
                  }

                  break;

            case KEY_LEFT:
                  if (n != 0)
                  {
                        flotte.echangeSelection(n, flotte.getNavirePrecedent(n));
                        n = flotte.getNavirePrecedent(n);
                        flotte.refreshPort(0);
                  }
                  break;

            case '\n':

                  flotte.estAuPort(n, false);
                  flotte.refreshPort(0);
                  placementNavire(n);
                  if (flotte.getEstAuPort(n) == false)
                  {
                        n = flotte.getPremierNavire();
                        flotte.echangeSelection(n, flotte.getPremierNavire());
                  }
                  flotte.refreshPort(0);
                  break;
            }
      }
}

void Grille::init()
{
      int const h = getHeightGrille();
      int const w = getWidthGrille();
      Case = new etat *[h];

      for (int i = 0; i < h; i++)
      {
            Case[i] = new etat[w];

            for (int j = 0; j < w * 2; j += 2)
            {
                  Case[i][j / 2] = VIDE;
                  fenetre.print(j, i, "_", BWHITE);
                  fenetre.print(j + 1, i, '_|', BWHITE);
                  usleep(500);
            }
      }
}

void Grille::refreshGrille()
{
      int maxX = getWidthGrille();
      int maxY = getHeightGrille();

      for (int x = 0; x < maxX; x++)
      {
            for (int y = 0; y < maxY; y++)
            {
                  switch (Case[y][x])
                  {
                  case VIDE:
                        fenetre.print(2 * x, y, '_', BWHITE);
                        fenetre.print(2 * x + 1, y, '_|', BWHITE);
                        break;
                  case VIDETOUCHE:

                        break;
                  case NAVIRE:
                        fenetre.print(2 * x, y, ' ', BBLUE);
                        fenetre.print(2 * x + 1, y, ' ', BBLUE);
                        break;
                  case TOUCHE:
                        fenetre.print(2 * x, y, ' ', BRED);
                        fenetre.print(2 * x + 1, y, ' ', BRED);
                        break;
                  case TOUCHECOULE:
                        fenetre.print(2 * x, y, ' ', BRED);
                        fenetre.print(2 * x + 1, y, ' ', BRED);
                        break;
                  }
            }
      }
}

void Grille::placementNavire(int n)
{
      refreshGrille();

      int fx = 0;
      int fy = 0;
      int ch;
      int x = 0;
      int y = 0;

      for (x = 0; x < fx + flotte.getWidthnavire(n); x++)
      {
            for (y = 0; y < fy + flotte.getHeightnavire(n); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        fenetre.print(2 * (x), y, ' ', BBLUE);
                        fenetre.print(2 * (x) + 1, y, ' ', BBLUE);
                  }
            }
      }

      x += 1 - flotte.getWidthnavire(n);
      y -= flotte.getHeightnavire(n);

      while ((ch = getch()) != '\n')
      {
            switch (ch)
            {
            case KEY_RIGHT:
                  if (x < getWidthGrille() - flotte.getWidthnavire(n))
                  {
                        x++;

                        moveNavire(n, x, y);
                  }

                  break;
            case KEY_LEFT:
                  if (x > 0)
                  {
                        x--;

                        moveNavire(n, x, y);
                  }

                  break;
            case KEY_UP:
                  if (y > 0)
                  {
                        y--;
                        moveNavire(n, x, y);
                  }
                  break;
            case KEY_DOWN:
                  if (y < getHeightGrille() - flotte.getHeightnavire(n))
                  {
                        y++;
                        moveNavire(n, x, y);
                  }
                  break;
            case 'q':
                  flotte.estAuPort(n, true);
                  refreshGrille();
                  return;

                  break;
            }
      }
}
void Grille::moveNavire(int n, int &sx, int &sy)
{

      int x = 0;
      int y = 0;

      refreshGrille();

      for (x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (y = 0; y < flotte.getHeightnavire(n); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        fenetre.print(2 * (sx) + 2 * x, sy + y, ' ', BBLUE);
                        fenetre.print(2 * (sx) + 1 + 2 * x, sy + y, ' ', BBLUE);
                  }
            }
      }
}

bool Grille::check(int n,int sx, int sy)
{
      int x = 0;
      int y = 0;

      for (x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (y = 0; x < flotte.getHeightnavire(n); y++)
            {
                  
            }
      }
}