#include "grille.h"
#include "window.h"
#include "flotte.h"
#include <unistd.h>
using namespace std;

Grille::Grille(int const h, int const w, int sx, int sy) : height(h), width(w), startx(sx), starty(sy), fenetre(h, w, sx, sy), flotte(5, 14, sx + 3, sy + 12)
{
      init(h, w);
}

Grille::~Grille() {}

void Grille::init(int const h, int const w)
{
      Case = new etat *[h];

      for (int i = 0; i < h; i++)
      {
            Case[i] = new etat[w / 2];

            for (int j = 0; j < w; j += 2)
            {
                  Case[i][j / 2] = VIDE;
                  fenetre.print(j, i, "_", BWHITE);
                  fenetre.print(j + 1, i, '|', BWHITE);
                  usleep(18000);
            }
      }
}

void Grille::placement(int t)
{
      int ch;
      char p = ' ';
      Color col = WBLUE;
      int x = width / 2, x2 = x + 1, y = height / 2;

      for (int i = 0; i < t; i++)
      {
            fenetre.print(x, y, p, col);
            fenetre.print(x2, y, p, col);
            y--;
      }

      while ((ch = getch()) != 'q')
      {

            switch (ch)
            {
            case '1':
                  col = BMAGENTA;
                  break;
            case '2':
                  col = WCYAN;
                  break;
            case 'c':
                  fenetre.clear();
                  break;
            case KEY_UP:
                  if (y != 0)
                  {
                        fenetre.print(x, y, ' ');
                        fenetre.print(x2, y, ' ');
                        fenetre.print(x, --y, p, col);
                        fenetre.print(x2, y, p, col);
                  }
                  break;
            case KEY_DOWN:
                  if (y != 9)
                  {
                        fenetre.print(x, y, ' ');
                        fenetre.print(x2, y, ' ');
                        fenetre.print(x, ++y, p, col);
                        fenetre.print(x2, y, p, col);
                  }
                  break;
            }
      }
}