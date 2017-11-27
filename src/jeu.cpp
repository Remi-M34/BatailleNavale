#include "../include/headers.h"

using namespace std;

#define H getHeightGrille()
#define W getWidthGrille()

Jeu::Jeu(int sx, int sy, int sxf, int syf, int sx2, int sy2, int sxf2, int syf2, int scoreHeight, int scoreWidth, int sxScore, int syScore) : joueur1(sx, sy, sxf, syf), joueur2(sx2, sy2, sxf2, syf2, 1), score(scoreHeight, scoreWidth, sxScore, syScore)
{
    initCouleurs();
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
        case 18:
            score.setCarBordure(ligne[0]);
            break;
        case 19:
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

    joueur1.flotte.initSelection();
    joueur1.selectionNavire();

    // Le joueur 2 place ses navires aléatoirement
    joueur2.selectionNavire();

    // joueur2.flotte.initSelection();
    // joueur2.selectionNavire();

    int k = 1;
    int d;
    int t;

    // while (1)
    // {
    //     tir(joueur1);
    //     tir(joueur2);
    // }

    while (k <= (H) * (W))
    {
        d = joueur1.destinationMissile();
        while (d > 0)
        {
            if (d == 1)
            {
                score.print(2, 1, "Touché!");
                score.print(2, 3, "joueur 1 rejoue");
                d = joueur1.destinationMissile();
            }
            else if (d == 2)
            {
                score.print(2, 1, "Coulé ");
                score.print(2, 3, "joueur 1 rejoue");
                d = joueur1.destinationMissile();
            }
        }

        cout << "unun" << endl;
        cout << "unun" << endl;
        cout << "unun" << endl;
        d = joueur2.destinationMissile();

        while (d > 0)
        {
            if (d == 1)
            {
                score.print(2, 1, "Touché!");
                score.print(2, 3, "joueur 2 rejoue");
                d = joueur2.destinationMissile();
            }
            else if (d == 2)
            {
                score.print(2, 1, "Coulé ");
                score.print(2, 3, "joueur 2 rejoue");
                d = joueur2.destinationMissile();
            }
        }
    }

    k = 0;

    while (k < H * W)
    {
        int d = joueur2.destinationMissile();
        if (d == 1)
        {
            score.print(2, 1, "Touché!");
            score.print(2, 3, "joueur 1 rejoue");
        }
        else if (d == 2)
        {
            score.print(2, 1, "Coulé ");
            score.print(2, 3, "joueur 1 rejoue");
        }
        k++;
    }
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