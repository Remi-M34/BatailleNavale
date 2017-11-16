#include "window.h"
#include "config.h"
#include "grille.h"
#include <unistd.h>
#include <algorithm>
#include <cmath>
using namespace std;

void myprogram()
{
    int ch;
    int const h = 10;
    int const w = 20;
    Window menu(5, max(2*getWidthGrille(),getWidthFlotte()*2-4), 6, 1);
  menu.setCouleurBordure(BRED);

    Window score(3,max(2*getWidthGrille(),(getWidthFlotte()*2)-4), 2 * max(7+getWidthFlotte(),5+ getWidthGrille()),13+getHeightGrille());

  score.setCouleurBordure(BGREEN);



    // Création des grilles et flottes dont les positions dépendent des dimensions calculées dans le fichier config
    Grille joueur(6, 9, 6, 12 + getHeightGrille());
    Grille ennemi(10 + 2 * max(getWidthFlotte(), getWidthGrille()), 9, 10 + 2 * max(getWidthFlotte(), getWidthGrille()), 1);

    while ((ch = getch()) != 'q')
    {
    }
}

int main()
{

    startProgramX();
    myprogram();
    stopProgramX();
    return 0;
}
