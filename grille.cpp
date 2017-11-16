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
                  fenetre.print(j, i, " ", BWHITE);
                  fenetre.print(j + 1, i, ' ', BWHITE);
                  usleep(7000);
            }
      }
}
