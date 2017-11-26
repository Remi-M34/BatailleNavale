#include "jeu.h"
#include "window.h"
#include "config.h"
#include "grille.h"
#include <unistd.h>
#include "string"
using namespace std;

#define H getHeightGrille()
#define W getWidthGrille()

Jeu::Jeu(int sx, int sy, int sxf, int syf, int sx2, int sy2, int sxf2, int syf2, int scoreHeight, int scoreWidth, int sxScore, int syScore) : joueur1(sx, sy, sxf, syf), joueur2(sx2, sy2, sxf2, syf2), score(scoreHeight, scoreWidth, sxScore, syScore)
{
}

Jeu::~Jeu() {}

void Jeu::placement()
{

    score.setCouleurBordure(WCYAN);

    joueur1.flotte.initSelection();
    joueur1.selectionNavire();

    // joueur2.flotte.initSelection();
    // joueur2.selectionNavire();

    int k = 1;

    while (k > 0)
    {
        int k = joueur1.destinationMissile();
        if (k == 1)
        {
            score.print(2, 1, "Touché!");
            score.print(2, 3, "joueur 1 rejoue");
        }
        else if (k == 2)
        {
            score.print(2, 1, "Coulé ");
            score.print(2, 3, "joueur 1 rejoue");
        }
    }
}