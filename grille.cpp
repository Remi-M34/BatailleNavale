#include "grille.h"
#include "window.h"
#include "flotte.h"
#include "config.h"
#include "navire.h"
#include "algorithm"

#include <unistd.h>
using namespace std;
#define H getHeightGrille()
#define W getWidthGrille()

Grille::Grille(int const sx, int const sy, int sxf, int syf) : fenetre(H, W * 2, sx, sy), flotte(sxf, syf)
{

      //Initialise la grille et rend toutes les cases de la grille VIDE
      init();

      initDecalage();
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
                  if (n != flotte.getDernierNavire())
                  {
                        flotte.echangeSelection(n, flotte.getNavireSuivant(n));
                        n = flotte.getNavireSuivant(n);
                        flotte.refreshPort(0);
                  }

                  break;

            case KEY_LEFT:
                  if (n != flotte.getPremierNavire())
                  {
                        flotte.echangeSelection(n, flotte.getNavirePrecedent(n));
                        n = flotte.getNavirePrecedent(n);
                        flotte.refreshPort(0);
                  }
                  break;

            case '\n':
                  flotte.estAuPort(n, false);
                  placementNavire(n);

                  if (flotte.getEstAuPort(n))
                  {
                        break;
                  }

                  flotte.initSelection();

                  n = flotte.getPremierNavire();
                  // flotte.echangeSelection(n, flotte.getPremierNavire());

                  flotte.refreshPort(0);
                  refreshGrille(0, 0);

                  break;
            }
      }
}

void Grille::init()
{
      int const h = H;
      int const w = W;
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

void Grille::refreshGrille(int fx, int fy)
{
      int maxX, maxY;
      maxX = min(W, fx + 7);
      maxY = min(H, fy + 7);
      if (fx < 0)
      {
            fx = 0;
      }
      if (fy < 0)
      {
            fy = 0;
      }

      // if (fx == 0)
      // {
      //       maxX = W;
      // }
      // if (fy == 0)
      // {
      //       maxY = H;
      // }

      for (int x = fx; x < maxX; x++)
      {
            for (int y = fy; y < maxY; y++)
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
                        fenetre.print(2 * x, y, '#', BBLUE);
                        fenetre.print(2 * x + 1, y, '#', BBLUE);
                        break;
                  case TOUCHECOULE:
                        fenetre.print(2 * x, y, '#', BRED);
                        fenetre.print(2 * x + 1, y, '#', BRED);
                        break;
                  }
            }
      }
}

void Grille::placementNavire(int n)
{
      refreshGrille(0, 0);

      int fx = (W - flotte.getWidthnavire(n)) / 2;
      int fy = (H - flotte.getHeightnavire(n)) / 2;
      int ch;
      int x = 0;
      int y = 0;

      char c = ' ';
      if (!(checkPlacement(n,fx,fy)))
      {
            c = '#';
      }

      for (x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (y = 0; y < flotte.getHeightnavire(n); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        fenetre.print(2 * (x + fx), y + fy, c, BBLUE);
                        fenetre.print(2 * (x + fx) + 1, y + fy, c, BBLUE);
                  }
            }
      }
      x = 0;
      y = 0;
      x = (W - flotte.getWidthnavire(n)) / 2;
      y = (H - flotte.getHeightnavire(n)) / 2;

      while ((ch = getch()) != 'q')
      {
            switch (ch)
            {
            case KEY_RIGHT:
                  if (x < W - flotte.getWidthnavire(n))
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

                  if (y < H - flotte.getHeightnavire(n))
                  {
                        y++;
                        moveNavire(n, x, y);
                  }

                  break;

            case ' ':
                  pivoteDroite(n, x, y);
                  moveNavire(n, x, y);
                  break;

            case '\n':

                  if (checkPlacement(n, x, y))
                  {
                        refreshGrille(0, 0);
                        validerNavire(n, x, y);
                        return;
                  }
                  else{
                        changeCouleur();
                  }

                  break;

            case 'b':
                  flotte.estAuPort(n, true);
                  refreshGrille(x, y);

                  return;

                  break;
            }
      }
}
void Grille::moveNavire(int n, int sx, int sy)
{

      int x = 0;
      int y = 0;
      char c = ' ';
      char c2 = ' |';
      if (!checkPlacement(n,sx,sy))
      {
            c = '#';
      }

      refreshGrille(sx - 1, sy - 1);

      for (x = 0; x < fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)); x++)
      {
            for (y = 0; y < fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        fenetre.print(2 * (sx) + 2 * x, sy + y, c, BBLUE);
                        fenetre.print(2 * (sx) + 1 + 2 * x, sy + y, c2, BBLUE);
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

void Grille::pivoteDroite(int n, int &x, int &y)
{
      int taille = fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n));

      for (int x = 0; x < taille / 2; x++)
      {

            for (int y = x; y < taille - x - 1; y++)
            {
                  int temp = navire[n][x][y];

                  navire[n][x][y] = navire[n][y][taille - 1 - x];

                  navire[n][y][taille - 1 - x] = navire[n][taille - 1 - x][taille - 1 - y];

                  navire[n][taille - 1 - x][taille - 1 - y] = navire[n][taille - 1 - y][x];

                  navire[n][taille - 1 - y][x] = temp;
            }
      }

      checkRepositionnement(n);

      cout << fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)) << endl;
      cout << fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)) << endl;
      cout << fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)) << endl;
      // refreshGrille();

      flotte.swapDimensionsNavire(n);

      if (x + flotte.getWidthnavire(n) > W)
      {
            x -= abs(flotte.getHeightnavire(n) - flotte.getWidthnavire(n));
            ;
      }
      if (y + flotte.getHeightnavire(n) > H)
      {
            y -= abs(flotte.getHeightnavire(n) - flotte.getWidthnavire(n));
      }
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
      for (x = 0; navire[n][x + 1][0] != -1; x++)
      {
            for (int y = 0; navire[n][0][y] != -1; y++)
            {
                  navire[n][x][y] = navire[n][x + 1][y];
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

bool Grille::checkPlacement(int n, int sx, int sy)
{
      for (int x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (int y = 0; y < flotte.getHeightnavire(n); y++)
            {

                  if (Case[sy + y][sx + x] == NAVIRE && navire[n][x][y] == 1)
                  {
                        return false;
                  }
            }
      }

      return true;
}

void Grille::initDecalage()
{
      for (int n = 0; n < 5; n++)
      {
      }
}

void Grille::validerNavire(int n, int sx, int sy)
{

      for (int y = 0; y < flotte.getHeightnavire(n); y++)
      {
            for (int x = 0; x < flotte.getWidthnavire(n); x++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        Case[sy + y][sx + x] = NAVIRE;
                  }
            }
      }
}

void Grille::changerEtat(int x, int y, etat e)
{
      Case[y][x] = e;
}




void Grille::changeCouleur()
{
 Color tmp = fenetre.getCouleurBordure();
 fenetre.setCouleurBordure(BRED);
 usleep(100000);
 fenetre.setCouleurBordure(WBLACK);


}