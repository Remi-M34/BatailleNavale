#include "../include/headers.h"

using namespace std;

#define H getHeightGrille()
#define W getWidthGrille()

Jeu::Jeu(int sx, int sy, int sxf, int syf, int sx2, int sy2, int sxf2, int syf2, int scoreHeight, int scoreWidth, int sxScore, int syScore, int xm, int ym) : joueur1(sx, sy, sxf, syf, 0),
                                                                                                                                                              joueur2(sx2, sy2, sxf2, syf2, 1),
                                                                                                                                                              score(scoreHeight, scoreWidth, sxScore, syScore),
                                                                                                                                                              scoreWidth(scoreWidth),
                                                                                                                                                              syf2(syf2), sxf2(sxf2), sy2(sy2), sx2(sx2)
{

    initCouleurs();

          Joueur[0] = new Grille(sx, sy, sxf, syf, 0);
          Joueur[1] = new Grille(sx2, sy2, sxf2, syf2, 1);


}

Jeu::~Jeu() {}

void Jeu::initCouleurs()
{

    ifstream couleurs("config/couleurs.txt", ios::in);
    int lignes = 1;
    string ligne;

    while (getline(couleurs, ligne))
    {

        switch (lignes)
        {
        case 17:
            colScore = convertColor(ligne);
            score.setCouleurBordure(colScore);
            break;
        case 19:
            score.setCarBordure(ligne[0]);
            break;
        case 20:
            if (ligne[0] == 'O' || ligne[0] == 'o')
            {
                score.setBordureDroite();
            }
            couleurs.close();
            return;
        }
        lignes++;
    }

    couleurs.close();
}

void Jeu::placement()
{

    // score.setBordureDroite();

    joueur1.selectionNavire();

    // Le joueur 2 place ses navires aléatoirement
    joueur2.selectionNavire();

    joueur2.flotte.fenetre.setBordureDroite();
    joueur2.flotte.fenetre.setCouleurBordure(WBLACK);
    joueur2.flotte.fenetre.setCarBordure(' ');

    Window help(6, 22, sx2, syf2 - 1, ' ');
    help.setCouleurBordure(BCYAN);
        help.print(9, 0, "AIDE");

    help.print(1, 2, "Case suivante      c");
    help.print(1, 3, "Pose aléatoire     a");

    // joueur2.flotte.initSelection();
    // joueur2.selectionNavire();

    int k = 1;
    int d;

    string score1 = to_string(scoreJoueur1);

    while (k <= (H) * (W))
    {
        score.print(6, 10, "SCORES");

        d = joueur1.destinationMissile();
        while (d > 0)
        {
            if (d == 1)
            {

                scoreplus(1, 2);
                score.print(1, 1, "Touché! +1 pt");
                score.print(1, 3, "Joueur 2 rejoue");
                d = joueur1.destinationMissile();
            }
            else if (d == 2)
            {
                scoreplus(2, 2);
                score.print(1, 1, "Coulé! +2 pts");
                score.print(1, 3, "Joueur 2 rejoue");
                d = joueur1.destinationMissile();
            }
        }

        d = joueur2.destinationMissile();

        while (d > 0)
        {
            if (d == 1)
            {
                scoreplus(1, 1);
                score.print(1, 1, "Touché! +1 pt");
                score.print(1, 3, "joueur 1 rejoue");
                d = joueur2.destinationMissile();
            }
            else if (d == 2)
            {

                scoreplus(2, 1);
                score.print(1, 1, "Coulé! +2 pts");
                score.print(1, 3, "Joueur 1 rejoue");
                d = joueur2.destinationMissile();
            }
        }
    }

    // while (k < H * W)
    // {
    //     int d = joueur2.destinationMissile();
    //     if (d == 1)
    //     {
    //         score.print(2, 1, "Touché!");
    //         score.print(2, 3, "joueur 1 rejoue");
    //     }
    //     else if (d == 2)
    //     {
    //         score.print(2, 1, "Coulé ");
    //         score.print(2, 3, "joueur 1 rejoue");
    //     }
    //     k++;
    // }
}

void Jeu::tir(Grille joueur)
{
    int d = joueur.destinationMissile();
    if (d == 1)
    {
        score.print(2, 1, "Touché!");
        score.print(2, 3, "joueur 1 rejoue");
        tir(joueur);
    }
    else if (d == 2)
    {
        score.print(2, 1, "Coulé ");
        score.print(2, 3, "joueur 1 rejoue");
        tir(joueur);
    }
}

void Jeu::scoreplus(int s, int j)
{
    switch (j)
    {
    case 1:
        switch (s)
        {
        case 1:
            scoreJoueur1++;
            break;
        case 2:
            scoreJoueur1 += 2;
            break;
        }
        score.print(1, 12, to_string(scoreJoueur1));
        break;
    case 2:
        switch (s)
        {
        case 1:
            scoreJoueur2++;
            break;
        case 2:
            scoreJoueur2 += 2;
            break;
        }
        score.print(16, 12, to_string(scoreJoueur2));
        break;
    }
}

void aideJeu()
{
    // Window help(6,20,sxf)
    // help.print(1,2,"c      Case suivante");
}