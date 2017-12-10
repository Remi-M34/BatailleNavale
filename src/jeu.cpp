#include "../include/jeu.h"
#include "../include/window.h"
#include "../include/grille.h"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

#define H getHeightGrille()
#define W getWidthGrille()
#define WF getDimFlotte('w')
#define HF getDimFlotte('h')

Jeu::Jeu(int nbjoueurs, int humains, int diff, int v) : nbjoueurs(nbjoueurs),
                                                        humains(humains),
                                                        info(20, 18, getScoreStartX(nbjoueurs), -(H + HF) / 1.9),
                                                        plateau(getBordureHeight(nbjoueurs), getBordureWidth(nbjoueurs), getScoreStartX(nbjoueurs) - 4, getBordureStartY(nbjoueurs)),
                                                        aide(7, 2 * W + 4, getScoreStartX(nbjoueurs) + 22, getAideStartY(nbjoueurs)),
                                                        difficulte(diff),
                                                        vitesse(v)

{
    // plateau.setCouleurBordure(BCYAN);
    initCouleurs();
    srand((int)time(0));

    initDim(nbjoueurs);
    curs_set(0);
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
        case 15:
            plateau.setCouleurBordure(convertColor(ligne));
            break;
        case 18:
            colScore = convertColor(ligne);
            info.setCouleurBordure(colScore);
            break;
        case 19:
            colAide = convertColor(ligne);
            info.setCouleurBordure(colAide);
            break;
        case 20:
            colBordureGrilles = convertColor(ligne);
            colBordurePlateau = convertColor(ligne);
            break;
        case 21:
            plateau.setCarBordure(ligne[0]);
            break;
        case 22:
            info.setCarBordure(ligne[0]);
            break;
        case 23:
            aide.setCarBordure(ligne[0]);
            break;
        case 24:
            plateau.setBordureDroite();
            break;
        case 25:
            if (ligne[0] == 'O' || ligne[0] == 'o')
            {
                info.setBordureDroite();
            }
            break;
        }
        lignes++;
    }

    couleurs.close();
}

void Jeu::start()
{

    placementDesNavires();
    (*Joueur[0]).flotte.fenetre.setBordureDroite();
    (*Joueur[0]).flotte.fenetre.setCarBordure(' ');
    info.setBordureDroite();
    // aide.setBordureDroite();
    aide.setCouleurBordure(BGREEN);
    aide.setCarBordure(' ');
    aide.print(1, 0, "Vitesse ( +, - )");
    aide.print(aide.getWindowWidth() - 2, 0, myitoa(vitesse), colBordureGrilles);
    aide.print(1, 1, "Selection");
    aide.print(aide.getWindowWidth() - 6, 1, "<-,->");

    aide.print(1, 2, "Annuler cible");
    aide.print(aide.getWindowWidth() - 2, 2, "c");

    aide.print(1, 3, "Case suivante");
    aide.print(aide.getWindowWidth() - 2, 3, "e");

    aide.print(1, 4, "Case précédente");
    aide.print(aide.getWindowWidth() - 2, 4, "z");
    aide.print(1, 5, "Tir aléatoire");
    aide.print(aide.getWindowWidth() - 2, 5, "a");

    (*Joueur[joueur]).setJoue(true);
    usleep(155555);

    selectionCible(joueur);
    // cibleSelectionnee = joueur;
    int ch;
    while (1)
    {
        refreshScores();
        // checkSpeed(ch,aide,vitesse);

        switch (attaque())
        {
        case 1:
            payback[cibleSelectionnee] = joueur;
            (*Joueur[joueur]).Score.navireTouche();
            classementUp();

            estVulnerable[cibleSelectionnee] = 1;
            continue;

        case 2:
            (payback[cibleSelectionnee]) = joueur;
            (*Joueur[joueur]).Score.navireTouche();
            classementUp();
            if ((*Joueur[cibleSelectionnee]).findFocus() == -1)
            {
                estVulnerable[cibleSelectionnee] = 0;
            }
            continue;

        case 0:
            (payback[cibleSelectionnee]) = joueur;
            (*Joueur[joueur]).Score.missileEnvoye();
            classementDown();
            joueurSuivant();
            usleep(max((9 - vitesse) * 27000, 40000));
            selectionCible(joueur);

            break;
        case -1:
            (*Joueur[cibleSelectionnee]).setCiblageValide(false);

            selectionCible(joueur);

            break;
        }
    }

    // HFile (k < H * W)
    // {
    //     int d = joueur2.destinationMissile();
    //     if (d == 1)
    //     {
    //         info.print(2, 1, "Touché!");
    //         info.print(2, 3, "joueur 1 rejoue");
    //     }
    //     else if (d == 2)
    //     {
    //         info.print(2, 1, "Coulé ");
    //         info.print(2, 3, "joueur 1 rejoue");
    //     }
    //     k++;
    // }
}

void Jeu::initDim(int nbjoueurs)
{

    // int 0 = COLS / 2;
    // int 0 = LINES / 2;

    int sx, sxf;
    if (W < WF)
    {
        sx = WF - W;
        sxf = 0;
    }
    else
    {
        sx = 0;
        sxf = W - WF;
    }

    switch (nbjoueurs)
    {
    case 2:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W, 0 - (H + HF) / 2, 0 - 2 - sxf - 2 * WF, 4 + H + 0 - (H + HF) / 2, vitesse, difficulte, aide, estIA());
        Joueur[1] = new Grille(0 + 2 + sx, 0 - (H + HF) / 2, 0 + 2 + sxf, 4 + H + 0 - (H + HF) / 2, vitesse, difficulte, aide, estIA());
        break;
    }
    case 3 ... 4:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W, 0 + 1, 0 - 2 - sxf - 2 * WF, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA());
        Joueur[1] = new Grille(0 + 2 + sx, 0 + 1, 0 + 2 + sxf, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA());

        switch (nbjoueurs)
        {
        case 3:
            Joueur[2] = new Grille(0 - 2 - sx - 2 * W, 0 - H - 2, 0 - 2 - sxf - 2 * WF, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA());
            break;
        case 4:
            Joueur[2] = new Grille(0 - 2 - sx - 2 * W, 0 - H - 2, 0 - 2 - sxf - 2 * WF, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA());
            Joueur[3] = new Grille(0 + 2 + sx, 0 - H - 2, 0 + 2 + sxf, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA());
            break;
        }
        break;
    }
    case 5 ... 6:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W - 12, 0 + 1, 0 - 2 - sxf - 2 * WF - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA());
        Joueur[1] = new Grille(0 + 2 + sx - 12, 0 + 1, 0 + 2 + sxf - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA());
        Joueur[2] = new Grille(0 + 6 + sx + 2 * max(W, WF) - 12, 0 + 1, 0 + 6 + sxf + 2 * max(W, WF) - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA());

        Joueur[3] = new Grille(0 - 2 - sx - 2 * W - 12, 0 - H - 2, 0 - 2 - sxf - 2 * WF - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA());
        Joueur[4] = new Grille(0 + 2 + sx - 12, 0 - H - 2, 0 + 2 + sxf - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA());

        if (nbjoueurs == 6)
        {
            Joueur[5] = new Grille(0 + 6 + sx + 2 * max(W, WF) - 12, 0 - H - 2, 0 + 6 + sxf + 2 * max(W, WF) - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA());
        }
        break;
    }
    }

    // *payback = new int[nbjoueurs];
    // *estVulnerable = new bool[nbjoueurs];

    // srand((int)time(0));
    // payback = new int[nbjoueurs];
    // estVulnerable = new int[nbjoueurs];

    for (int i = 0; i < nbjoueurs; i++)
    {
        payback[i] = rand() % nbjoueurs;
        estVulnerable[i] = 0;
        positionDuJoueur[i] = i;
        joueurEnPosition[i] = i;

        // estVulnerable[i] = 0;
    }
}

bool Jeu::estIA()
{
    humains--;
    if (humains >= 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Jeu::selectionCible(int j)
{

    if ((*Joueur[joueur]).estIA())
    {
        selectionCibleAleatoire();
        return;
    }

    int c;
    while (c = getch())
    {
        switch (c)
        {
        case 'n':
            navires();
            for (int i = 0; i < nbjoueurs; i++)
            {
                (*Joueur[i]).fenetre.setCouleurBordure(BCYAN);

                if (i == cibleSelectionnee)
                {
                    (*Joueur[cibleSelectionnee]).setEstCible(true);
                }
            }
            for (int i = 0; i < nbjoueurs; i++)
            {
                (*Joueur[i]).refreshGrille(0, 0, W, H);
            }

            break;
        case KEY_RIGHT:
        {
            selectRight();
            break;
        }
        case KEY_LEFT:
        {
            selectLeft();
            break;
        }
        case KEY_UP:

        {
            if (nbjoueurs > 2)
            {
                selectUp();
            }
            break;
        }

        case KEY_DOWN:
        {
            if (nbjoueurs > 2)
            {
                selectDown();
            }
            break;
        }
        case 'a':
        {
            selectionCibleAleatoire();
            break;
        }
        case '\n':
            if (joueur != cibleSelectionnee)
            {
                (*Joueur[cibleSelectionnee]).setCiblageValide(true);
                return;
            }
            else
            {
                (*Joueur[joueur]).mauvaiseSelection();
            }

            break;
        }
    }
}

int Jeu::joueurVulnerable()
{
    int k = 0;
    int vul[nbjoueurs] = {-1};
    for (int i = 0; i < nbjoueurs; i++)
    {
        if ((estVulnerable[i] == 1) && joueur != i)
        {
            vul[k] = i;
            k++;
        }
    }
    if (k == 0)
        return rand() % nbjoueurs;

    return vul[rand() % k];
}

void Jeu::cibleAleatoire()
{

    do
    {
        cible = rand() % nbjoueurs;
    } while (cible == joueur);
}

void Jeu::selectionCibleAleatoire()
{

    do
    {
    test("rand...");

        switch (difficulte)
        {
        case 3:

            cible = joueurVulnerable();
            break;
        case 2:
            if ((rand() % 2) == 0)
            {
                cible = joueurVulnerable();
            }
            else
            {
                cible = payback[joueur];
            }
            break;
        case 1:
            switch (int k = (rand() % 3))
            {
            case 0:
                cible = joueurVulnerable();
                break;
            case 1:
                cible = payback[joueur];
                break;
            case 2:
                cibleAleatoire();
                break;
            }

            break;
        }

        // cible = rand() % nbjoueurs;
    } while (cible == joueur);
    test("\n");

    test(myitoa(cible));
    test("est la cible. Depuis le joueur ");
    test(myitoa(joueur));
    // (*Joueur[cibleSelectionnee]).setEstCible();
    {
        deplacementIA();
    }
}

void Jeu::deplacementIA()
{

    switch (cibleSelectionnee)
    {
    case 0 ... 2:
        switch (cible)
        {
        case 0 ... 2:
            if (cibleSelectionnee > cible)
            {
                selectLeft();
            }
            else
            {
                selectRight();
            }
            break;
        case 3 ... 5:
        {
            if (nbjoueurs > 4)
                selectUp();
            else
                selectRight();
        }
        break;
        }
        break;
    case 3 ... 5:

        switch (cible)
        {
        case 0 ... 2:
            selectDown();
            break;
        case 3 ... 5:
            if (cibleSelectionnee > cible)
            {
                selectLeft();
            }
            else
            {
                selectRight();
            }
            break;
        }
        break;
    }
    usleep(min((9 - vitesse) * 30000, 333333));
    if (cible != cibleSelectionnee)
    {
        deplacementIA();
    }
    else
    {
        return;
    }
}

void Jeu::selectUp()
{
    if (nbjoueurs > 4)
    {
        switch (cibleSelectionnee)
        {
        case 0:
            (*Joueur[cibleSelectionnee]).setEstCible(false);
            (*Joueur[3]).setEstCible(true);
            cibleSelectionnee = 3;
            return;
        case 1:
            if (nbjoueurs > 4)
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[4]).setEstCible(true);
                cibleSelectionnee = 4;
            }
            else
            {
                selectLeft();
            }
            return;
        case 2:
            if (nbjoueurs == 6)
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[5]).setEstCible(true);
                cibleSelectionnee = 5;
            }
            else
            {
                selectLeft();
            }
            return;
        }
    }
    else if (nbjoueurs <= 4)
    {
        switch (cibleSelectionnee)
        {
        case 0:
            (*Joueur[cibleSelectionnee]).setEstCible(false);
            (*Joueur[2]).setEstCible(true);
            cibleSelectionnee = 2;
            return;
        case 1:
            if (nbjoueurs == 4)
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[3]).setEstCible(true);
                cibleSelectionnee = 3;
            }
            return;
        }
    }
    else
    {
        selectLeft();
    }
}

void Jeu::selectDown()
{
    if (nbjoueurs > 4)
    {
        switch (cibleSelectionnee)
        {
        case 3:
            (*Joueur[cibleSelectionnee]).setEstCible(false);
            (*Joueur[0]).setEstCible(true);
            cibleSelectionnee = 0;
            break;
        case 4:
            if (nbjoueurs > 4)
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[1]).setEstCible(true);
                cibleSelectionnee = 1;
            }
            break;
        case 5:
            if (nbjoueurs == 6)
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[2]).setEstCible(true);
                cibleSelectionnee = 2;
            }
            break;
        }
    }
    else if (nbjoueurs <= 4)
    {
        switch (cibleSelectionnee)
        {
        case 3:
            (*Joueur[cibleSelectionnee]).setEstCible(false);
            (*Joueur[1]).setEstCible(true);
            cibleSelectionnee = 1;
            break;
        case 2:
        {
            (*Joueur[cibleSelectionnee]).setEstCible(false);
            (*Joueur[0]).setEstCible(true);
            cibleSelectionnee = 0;
        }
        break;
        }
    }
}

void Jeu::selectRight()
{
    if (cibleSelectionnee < nbjoueurs - 1)
    {
        (*Joueur[cibleSelectionnee]).setEstCible(false);
        (*Joueur[++cibleSelectionnee]).setEstCible(true);
    }
}

void Jeu::selectLeft()
{
    if (cibleSelectionnee > 0)
    {
        (*Joueur[cibleSelectionnee]).setEstCible(false);
        (*Joueur[--cibleSelectionnee]).setEstCible(true);
    }
}

void Jeu::placementDesNavires()
{
    for (int i = 0; i < nbjoueurs; i++)
    {
        info.print(1, 0, 'Joueur ');
        info.print(8, 0, to_string(i));
        Joueur[i]->selectionNavire();
    }
}

void Jeu::joueurSuivant()
{
    (*Joueur[cibleSelectionnee]).setEstCible(false);

    (*Joueur[joueur]).setJoue(false);

    if (joueur < nbjoueurs - 1)
    {
        joueur++;
    }
    else
    {
        joueur = 0;
    }
    (*Joueur[joueur]).setJoue(true);
    cibleSelectionnee = joueur;
    usleep(min((9 - vitesse) * 20000, 50000));
}

int Jeu::attaque()
{
    if ((*Joueur[joueur]).estIA())
    {
        (*Joueur[cibleSelectionnee]).setCiblageValide(true);
        return (*Joueur[cibleSelectionnee]).destinationMissileAleatoire();
    }
    else
    {
        return (*Joueur[cibleSelectionnee]).destinationMissile();
    }
}

int getScoreStartX(int nbjoueurs)
{

    if (nbjoueurs < 5)
    {
        return (-2 * max(W, WF) - 26 + max(0, WF - W));
    }
    else
    {
        return (-2 * max(W, WF) - 38 + max(0, WF - W));
    }
}

int getBordureWidth(int nbjoueurs)
{
    switch (nbjoueurs)
    {
    case 2 ... 4:
    {
        return 36 + 4 * max(W, WF) - max(0, WF - W);
    }
    case 5 ... 6:
    {
        return 40 + 6 * max(W, WF) - max(0, WF - W);
    }
    }
}

int getBordureHeight(int nbjoueurs)
{
    if (nbjoueurs > 2)
    {
        return 16 + 2 * (H + HF) + 6 - HF;
    }
    else
    {
        return 12 + (H + HF) + 6 - HF;
    }
}

int getBordureStartY(int nbjoueurs)
{
    if (nbjoueurs > 2)
    {
        return -9 - H - HF;
    }
    else
    {
        return -6 - (H) / 2;
    }
}

int getAideStartY(int nbjoueurs)
{
    if (nbjoueurs == 2)
        return (-(H + HF) / 1.9) + 3 + H + HF / 2 + max(0, 3 - HF);
    else
        return 4 + H;
}

void Jeu::navires()
{
    for (int i = 0; i < nbjoueurs; i++)
    {

        (*Joueur[i]).cacherCases();
    }
    Flotte nav(COLS / 2, LINES / 2, 0);
    cin.ignore(1);
    nav.fenetre.setBordureDroite();
    nav.fenetre.setCarBordure(' ');
}

void checkSpeed(int ch, Window &aide, int &vitesse)
{
    for (int i = 0; i < 5; i++)
    {
        switch (ch)
        {
        case '+':
            vitesse++;
            break;
        case '-':
            vitesse--;
            break;
        }
    }
    aide.print(aide.getWindowWidth() - 2, 0, myitoa(vitesse), BCYAN);
}

void Jeu::refreshScores()
{

    for (int i = 0; i < nbjoueurs; i++)
    {
        string t = myitoa((*Joueur[joueurEnPosition[i]]).Score.getScore());
        switch (t.length())
        {
            case 4:
            t.insert(1," ");
            break;
                        case 5:
            t.insert(2," ");
            break;
                        case 6:
            t.insert(3," ");
            break;
        }

        string str = "Joueur " + myitoa(joueurEnPosition[i]) + ":";
        string str2 = myitoa((*Joueur[joueurEnPosition[i]]).Score.getScore()/1000)+" "+myitoa((*Joueur[joueurEnPosition[i]]).Score.getScore()%1000);
                info.print(0, 15 + i, "                    ");
        info.print(0, 15 + i, str);
        info.print(18-t.length(), 15 + i, t);
        cout <<i << endl;
        // info.print(1,13+i,myitoa(i));
    }
}

void Jeu::classementDown()
{

        if (positionDuJoueur[joueur] < nbjoueurs-1 && (*Joueur[joueur]).getScore() < (*Joueur[joueurEnPosition[positionDuJoueur[joueur] + 1 ]]).getScore())
        {
                        int pos = joueurEnPosition[positionDuJoueur[joueur] + 1];

            swap(joueurEnPosition[positionDuJoueur[joueur]], joueurEnPosition[positionDuJoueur[joueur] + 1 ]);

            swap(positionDuJoueur[joueur],positionDuJoueur[pos]);

            // positionDuJoueur[joueur]++;
            // positionDuJoueur[joueurEnPosition[positionDuJoueur[joueur]]]--;
            test("\nLe joueur ");
            test(myitoa(joueur));
                        test("\n devient  ");
            test(myitoa(positionDuJoueur[joueur]));
            test("      et le joueur ");
            test(myitoa(joueurEnPosition[positionDuJoueur[joueur]]));
            test(" devient ");
            test(myitoa(positionDuJoueur[joueurEnPosition[positionDuJoueur[joueur]]]));
            classementDown();

        }

    
}


void Jeu::classementUp()
{

        if (positionDuJoueur[joueur] > 0 && (*Joueur[joueur]).getScore() > (*Joueur[joueurEnPosition[positionDuJoueur[joueur] - 1 ]]).getScore())
        {
            int pos = joueurEnPosition[positionDuJoueur[joueur] - 1];
            swap(joueurEnPosition[positionDuJoueur[joueur]], joueurEnPosition[positionDuJoueur[joueur] - 1 ]);

            swap(positionDuJoueur[joueur],positionDuJoueur[pos]);

            // positionDuJoueur[joueur]++;
            // positionDuJoueur[joueurEnPosition[positionDuJoueur[joueur]]]--;
            test("\nLe joueur ");
            test(myitoa(joueur));
                        test("\n devient  ");
            test(myitoa(positionDuJoueur[joueur]));
            test("      et le joueur ");
            test(myitoa(joueurEnPosition[positionDuJoueur[joueur]]));
            test(" devient ");
            test(myitoa(positionDuJoueur[joueurEnPosition[positionDuJoueur[joueur]]]));
            classementUp();

        }

}