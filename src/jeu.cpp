#include "../include/headers.h"

using namespace std;

#define H getHeightGrille()
#define W getWidthGrille()
#define WF getDimFlotte('w')
#define HF getDimFlotte('h')

Jeu::Jeu(int nbjoueurs, int humains) : nbjoueurs(nbjoueurs),
                                       humains(humains),
                                       score(20, 18, getScoreStartX(nbjoueurs), LINES / 2 - (H + HF) / 1.9),
                                       bordure(getBordureHeight(nbjoueurs), getBordureWidth(nbjoueurs), getScoreStartX(nbjoueurs) - 4, getBordureStartY(nbjoueurs)),
                                       aide(6,2*W,getScoreStartX(nbjoueurs)+24,getAideStartY(nbjoueurs))
{
    vitesse = 3;
    bordure.setCouleurBordure(BCYAN);
    initCouleurs();
    initDim2(nbjoueurs);
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
            colAide = convertColor(ligne);
            score.setCouleurBordure(colAide);
            break;
        case 20:
            score.setCarBordure(ligne[0]);
            break;
                    case 21:
            aide.setCarBordure(ligne[0]);
            break;
        case 22:
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

    placementDesNavires();
        (*Joueur[0]).flotte.fenetre.setBordureDroite();
        (*Joueur[0]).flotte.fenetre.setCarBordure(' ');
    score.setBordureDroite();
    // aide.setBordureDroite();
    aide.setCouleurBordure(BGREEN);
    aide.setCarBordure(' ');
    aide.print(1,0,"Vitesse ( +, - )"); aide.print(aide.getWindowWidth()-2,0,myitoa(vitesse), BCYAN);
        aide.print(1,1,"Selection");    aide.print(aide.getWindowWidth()-6,1,"<-,->");

    aide.print(1,2,"Annuler cible");    aide.print(aide.getWindowWidth()-2,2,"c");


    aide.print(1,3,"Case suivante");    aide.print(aide.getWindowWidth()-2,3,"d");

    aide.print(1,4,"Case précédente");aide.print(aide.getWindowWidth()-2,4,"q");
    aide.print(1,5,"Tir aléatoire"); aide.print(aide.getWindowWidth()-2,5,"a");
    
    
    (*Joueur[joueur]).joue();
    usleep(155555);

    selectionCible(joueur);
    // cibleSelectionnee = joueur;

    while (1)
    {

        switch (attaque())
        {
        case 1 ... 2:
            (*payback[cibleSelectionnee]) = joueur;
            continue;
        case 0:
            (*payback[cibleSelectionnee]) = joueur;

            (*Joueur[cibleSelectionnee]).nonCible();
            (*Joueur[cibleSelectionnee]).ciblageAnnule();

            joueurSuivant();
            usleep(50000);
            // cibleSelectionnee = (*payback[joueur]);
            cibleSelectionnee = joueur;

            selectionCible(joueur);

            break;
        case -1:
            (*Joueur[cibleSelectionnee]).ciblageAnnule();

            selectionCible(joueur);

            break;
        }
    }

    // HFile (k < H * W)
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

void Jeu::initDim2(int nbjoueurs)
{

    int xm = COLS / 2;
    int ym = LINES / 2;

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
        Joueur[0] = new Grille(xm - 2 - sx - 2 * W, ym - (H + HF) / 2, xm - 2 - sxf - 2 * WF, 4 + H + ym - (H + HF) / 2, vitesse,aide, estIA());
        Joueur[1] = new Grille(xm + 2 + sx, ym - (H + HF) / 2, xm + 2 + sxf, 4 + H + ym - (H + HF) / 2, vitesse,aide,estIA());
        break;
    }
    case 3 ... 4:
    {
        Joueur[0] = new Grille(xm - 2 - sx - 2 * W, ym + 1, xm - 2 - sxf - 2 * WF, 4 + H + ym + 1, vitesse,aide,estIA());
        Joueur[1] = new Grille(xm + 2 + sx, ym + 1, xm + 2 + sxf, 4 + H + ym + 1, vitesse,aide,estIA());

        switch (nbjoueurs)
        {
        case 3:
            Joueur[2] = new Grille(xm - 2 - sx - 2 * W, ym - H - 2, xm - 2 - sxf - 2 * WF, ym - 6 - H - HF, vitesse,aide,estIA());
            break;
        case 4:
            Joueur[2] = new Grille(xm - 2 - sx - 2 * W, ym - H - 2, xm - 2 - sxf - 2 * WF, ym - 6 - H - HF, vitesse,aide,estIA());
            Joueur[3] = new Grille(xm + 2 + sx, ym - H - 2, xm + 2 + sxf, ym - 6 - H - HF, vitesse,aide,estIA());
            break;
        }
        break;
    }
    case 5 ... 6:
    {
        Joueur[0] = new Grille(xm - 2 - sx - 2 * W - 12, ym + 1, xm - 2 - sxf - 2 * WF - 12, 4 + H + ym + 1, vitesse,aide,estIA());
        Joueur[1] = new Grille(xm + 2 + sx - 12, ym + 1, xm + 2 + sxf - 12, 4 + H + ym + 1, vitesse,aide,estIA());
        Joueur[2] = new Grille(xm + 6 + sx + 2 * max(W, WF) - 12, ym + 1, xm + 6 + sxf + 2 * max(W, WF) - 12, 4 + H + ym + 1, vitesse,aide,estIA());

        Joueur[3] = new Grille(xm - 2 - sx - 2 * W - 12, ym - H - 2, xm - 2 - sxf - 2 * WF - 12, ym - 6 - H - HF, vitesse,aide,estIA());
        Joueur[4] = new Grille(xm + 2 + sx - 12, ym - H - 2, xm + 2 + sxf - 12, ym - 6 - H - HF, vitesse,aide,estIA());

        if (nbjoueurs == 6)
        {
            Joueur[5] = new Grille(xm + 6 + sx + 2 * max(W, WF) - 12, ym - H - 2, xm + 6 + sxf + 2 * max(W, WF) - 12, ym - 6 - H - HF, vitesse,aide,estIA());
            break;
        }
    }
    }

    for (int i = 0; i < nbjoueurs; i++)
    {
        (*payback[i]) = rand() % nbjoueurs;
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
    while ((c = getch()) != '$')
    {
        switch (c)
        {
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
            if (nbjoueurs > 3)
{            selectUp();
}
            break;
        }

        case KEY_DOWN:
        {
            if (nbjoueurs > 3)
{            selectDown();
}
            break;
        }
        case '\n':
            (*Joueur[cibleSelectionnee]).ciblageValide();
            return;

            break;
        }
    }
}

void Jeu::selectionCibleAleatoire()
{
    do
    {
        (*Joueur[0]).test("rand...");

        cible = rand() % nbjoueurs;
    } while (cible == joueur);
    (*Joueur[0]).test(myitoa(cible));
    (*Joueur[0]).test("est la cible. Depuis le joueur ");
    (*Joueur[0]).test(myitoa(joueur));
    // (*Joueur[cibleSelectionnee]).estCible();
    deplacementIA();
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
    usleep(333333);
    if (cible != cibleSelectionnee)
    {
        deplacementIA();
        cout << "ok" << endl;
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
            (*Joueur[cibleSelectionnee]).estCible();
            (*Joueur[3]).estCible();
            cibleSelectionnee = 3;
            return;
        case 1:
            if (nbjoueurs > 4)
            {
                (*Joueur[cibleSelectionnee]).estCible();
                (*Joueur[4]).estCible();
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
                (*Joueur[cibleSelectionnee]).estCible();
                (*Joueur[5]).estCible();
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
            (*Joueur[cibleSelectionnee]).estCible();
            (*Joueur[2]).estCible();
            cibleSelectionnee = 2;
            return;
        case 1:
            // if (nbjoueurs >  4)
            {
                (*Joueur[cibleSelectionnee]).estCible();
                (*Joueur[3]).estCible();
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
            (*Joueur[cibleSelectionnee]).estCible();
            (*Joueur[0]).estCible();
            cibleSelectionnee = 0;
            break;
        case 4:
            if (nbjoueurs > 4)
            {
                (*Joueur[cibleSelectionnee]).estCible();
                (*Joueur[1]).estCible();
                cibleSelectionnee = 1;
            }
            break;
        case 5:
            if (nbjoueurs == 6)
            {
                (*Joueur[cibleSelectionnee]).estCible();
                (*Joueur[2]).estCible();
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
            (*Joueur[cibleSelectionnee]).estCible();
            (*Joueur[1]).estCible();
            cibleSelectionnee = 1;
            break;
        case 2:
        {
            (*Joueur[cibleSelectionnee]).estCible();
            (*Joueur[0]).estCible();
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
        (*Joueur[cibleSelectionnee]).estCible();
        (*Joueur[++cibleSelectionnee]).estCible();
    }
}

void Jeu::selectLeft()
{
    if (cibleSelectionnee > 0)
    {
        (*Joueur[cibleSelectionnee]).estCible();
        (*Joueur[--cibleSelectionnee]).estCible();
    }
}

void Jeu::placementDesNavires()
{
    for (int i = 0; i < nbjoueurs; i++)
    {
        score.print(1, 0, 'Joueur ');
        score.print(8, 0, to_string(i));
        Joueur[i]->selectionNavire();
    }
}

void Jeu::joueurSuivant()
{
    if (joueur < nbjoueurs - 1)
    {
        joueur++;
        (*Joueur[joueur]).joue();

        return;
    }
    joueur = 0;
    (*Joueur[joueur]).joue();
}

int Jeu::attaque()
{
    if ((*Joueur[joueur]).estIA())
    {
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
        return (COLS / 2 - 2 * max(W, WF) - 26 + max(0, WF - W));
    }
    else
    {
        return (COLS / 2 - 2 * max(W, WF) - 38 + max(0, WF - W));
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
        return 16 + 2 * (H + HF)+6-HF;
    }
    else
    {
        return 12 + (H + HF);
    }
}

int getBordureStartY(int nbjoueurs)
{
    if (nbjoueurs > 2)
    {
        return LINES / 2 - 9 - H - HF;
    }
    else
    {
        return LINES / 2 - 6 - (H) / 2;
    }
}


int getAideStartY(int nbjoueurs)
{
    if (nbjoueurs == 2)
    return (LINES / 2 - (H + HF) / 1.8)+2+H+HF/2;
    else
    return 4 + H + LINES/2 ;
}