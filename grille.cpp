#include "grille.h"
#include "window.h"
#include "flotte.h"
#include "config.h"
#include "navire.h"
#include "algorithm"

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

      x -= flotte.getWidthnavire(n);
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

                                    cout << x << "  " ;


                  break;
            case KEY_LEFT:
                  if (x > 0)
                  {
                        x--;

                        moveNavire(n, x, y);
                  }

                                    cout << x << "  " ;


                  break;
            case KEY_UP:
                  if (y > 0)
                  {
                        y--;
                        moveNavire(n, x, y);
                  }

                                    cout << x << "  " ;

                  break;
            case KEY_DOWN:
                  if (y < getHeightGrille() - flotte.getHeightnavire(n))
                  {
                        y++;
                        moveNavire(n, x, y);
                  }

                                    cout << x << "  " ;


                  break;

            case ' ':
                  pivoteDroite(n);
                  moveNavire(n, x, y);
                  cout << x << "  " << endl;
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

      for (x = 0; x < fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)); x++)
      {
            for (y = 0; y < fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        fenetre.print(2 * (sx) + 2 * x, sy + y, ' ', BBLUE);
                        fenetre.print(2 * (sx) + 1 + 2 * x, sy + y, ' ', BBLUE);
                  }
            }
      }
}

bool Grille::check(int n, int sx, int sy)
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

void Grille::pivoteDroite(int n)
{
      int taille = fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n));

      // Consider all squares one by one
      for (int x = 0; x < taille / 2; x++)
      {
            // Consider elements in group of 4 in
            // current square
            for (int y = x; y < taille - x - 1; y++)
            {
                  // store current cell in temp variable
                  int temp = navire[n][x][y];

                  // move values from right to top
                  navire[n][x][y] = navire[n][y][taille - 1 - x];

                  // move values from bottom to right
                  navire[n][y][taille - 1 - x] = navire[n][taille - 1 - x][taille - 1 - y];

                  // move values from left to bottom
                  navire[n][taille - 1 - x][taille - 1 - y] = navire[n][taille - 1 - y][x];

                  // assign temp to left
                  navire[n][taille - 1 - y][x] = temp;
            }
      }

      checkRepositionnement(n);

      cout << fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)) << endl;
      cout << fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)) << endl;
      cout << fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)) << endl;
      refreshGrille();
}

void Grille::checkRepositionnement(int n)
{

      if (flotte.getHeightnavire(n) != flotte.getWidthnavire(n))
      {
            int k = 0;
            for (int x = 0; navire[n][x][0] != -1; x++)
            {
                  k += navire[n][x][0];
            }
            if (k == 0)
            {
                  repositionnementVertical(n);
                  checkRepositionnement(n);
            }
            k = 0;
            for (int y = 0; navire[n][0][y] != -1; y++)
            {
                  k += navire[n][0][y];
            }
            if (k == 0)
            {
                  repositionnementHorizontal(n);
                  checkRepositionnement(n);
            }
      }
}

void Grille::repositionnementHorizontal(int n)
{
      int x = 0;
      for (x = 0; navire[n][x+1][0] != -1; x++)
      {
            for (int y = 0; navire[n][0][y] != -1; y++)
            {
                  navire[n][x][y] = navire[n][x+1][y];
            }
      }

      for (int y = 0; navire[n][0][y] != -1; y++)
      {
            navire[n][x][y] = 0;
      }
}

void Grille::repositionnementVertical(int n)
{
      int y = 0;
      for (int x = 0; navire[n][x][0] != -1; x++)
      {
            for (y = 0; navire[n][0][y + 1] != -1; y++)
            {
                  navire[n][x][y] = navire[n][x][y + 1];
            }
      }

      for (int x = 0; navire[n][x][0] != -1; x++)
      {
            navire[n][x][y] = 0;
      }
}

