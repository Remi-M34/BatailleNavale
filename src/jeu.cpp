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

Jeu::Jeu(int nbjoueurs, int humains, int diff, int v, string n[]) : nbjoueurs(nbjoueurs),
                                                                    humains(humains),
                                                                    info(22, 22, getScoreStartX(nbjoueurs) - 2, -(H + HF) / 1.9),
                                                                    plateau(getBordureHeight(nbjoueurs), getBordureWidth(nbjoueurs), getScoreStartX(nbjoueurs) - 4, getBordureStartY(nbjoueurs)),
                                                                    aide(7, 2 * W + 4, getScoreStartX(nbjoueurs) + 22, getAideStartY(nbjoueurs)),
                                                                    difficulte(diff),
                                                                    vitesse(v), JoueursRestants(nbjoueurs)

{

    // plateau.setCouleurBordure(BCYAN);
    for (int i = 0; i < 6; i++)
    {
        nom[i] = n[i];
    }
    initCouleurs();
    srand((int)time(0));

    initDim(nbjoueurs);
    curs_set(0);
    this->humains = humains;
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
            if (ligne[0] == 'O' || ligne[0] == 'o')
            {
                plateau.setBordureDroite();
            }
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

void Jeu::setupaide()
{
    // aide.clear();

    // aide.setCouleurBordure(BGREEN);
    // aide.setCarBordure(' ');
    aide.setBordureDroite();

    aide.print(1, 0, "Vitesse ( +, - )", CBLACK);
    aide.print(aide.getWindowWidth() - 2, 0, myitoa(vitesse), CBLACK);

    if (humains == 0)
    {
        aide.print(0, 2, "Mode spectateur.\nRéglez la vitesse du jeu avec les touches + et -.", WBLACK);
    }
    else
    {
        aide.print(1, 1, "Selection", CBLACK);
        aide.print(aide.getWindowWidth() - 6, 1, "<-,->");

        aide.print(1, 2, "Annuler cible", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 2, "c");

        aide.print(1, 3, "Case suivante", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 3, "e");

        aide.print(1, 4, "Case précédente", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 4, "z");
        aide.print(1, 5, "Tir aléatoire", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 5, "a");
    }
    DecalerHistorique(" Le jeu commence !", 0);
}

void Jeu::start()
{

    DecalerHistorique(" Les joueurs peuvent \ndésormais placer leur navires sur leur territoire!", 0);
    placementDesNavires();
    setupaide();

    (*Joueur[joueur]).setJoue(true);
    usleep(150000);

    selectionCible(joueur);
    // cibleSelectionnee = joueur;
    int ch;
    while (JoueursRestants > 1)
    {
        tour++;
        refreshScores();
        // checkSpeed(ch,aide,vitesse);

        switch (attaque())
        {
        case 1:
            InfoJeu(joueur, cibleSelectionnee, 1);
            payback[cibleSelectionnee] = joueur;
            (*Joueur[joueur]).Score.navireTouche();
            classementUp();

            estVulnerable[cibleSelectionnee] = 1;
            continue;

        case 2:
            InfoJeu(joueur, cibleSelectionnee, 2);

            (payback[cibleSelectionnee]) = joueur;
            (*Joueur[joueur]).Score.navireTouche();
            classementUp();
            if ((*Joueur[cibleSelectionnee]).findFocus() == -1)
            {
                estVulnerable[cibleSelectionnee] = 0;
            }
            continue;
        case 3:
            InfoJeu(joueur, cibleSelectionnee, 3);
            JoueursRestants--;
            (*Joueur[joueur]).Score.navireTouche();
            classementUp();
selectionCible(joueur);
break;
        case 0:
            (payback[cibleSelectionnee]) = joueur;
            (*Joueur[joueur]).Score.missileEnvoye();
            classementDown();
            joueurSuivant();
            usleep(max((9 - vitesse) * 27000, 0));
            selectionCible(joueur);

            break;
        case -1:
            (*Joueur[cibleSelectionnee]).setCiblageValide(false);
            tour--;
            selectionCible(joueur);

            break;
        }
    }

    while(1)
    {

    }
    
}

void Jeu::DecalerHistorique(string nouvelleinfo, int sc)
{
    info.setBordureDroite();
    // if (nouvelleinfo[66/3] == ' ')
    // {
    //     nouvelleinfo.erase(nouvelleinfo.begin()+22);
    // }

    for (int i = 5; i > 0; i--)
    {
        historique[i] = historique[i - 1];
        separateur[i] = separateur[i - 1];
        if (nouvelleinfo.length() > (6 - i) * (22) && nouvelleinfo[(6 - i) * (22)] == ' ')
        {
            nouvelleinfo.erase(nouvelleinfo.begin() + 110 / i);
        }
    }
    historique[0] = nouvelleinfo;

    switch (sc)
    {
    case -1:
        separateur[0] = WBLACK;
        break;
    case 0:
        separateur[0] = YBLACK;
        break;
    case 1:
        separateur[0] = BLUEBLACK;
        break;

    case 2:
        separateur[0] = MBLACK;
        break;
    }

    werase(info.win);
    info.print(0, 2 + nbjoueurs, "                    ", WBLACK);
    int ligne = 0;

    wattron(info.win, A_UNDERLINE);
    wattron(info.win, A_BOLD);

    info.print(6, 1 + nbjoueurs, "HISTORIQUE", GBLACK);
    wattroff(info.win, A_UNDERLINE);
    nbhisto += 1;

    for (int i = 0; i < 6 && nbhisto > i; i++)
    {
        mvwaddch(info.frame, ligne + 4 + nbjoueurs, 0, ACS_BULLET);
        info.print(0, ligne + 3 + nbjoueurs, historique[i], separateur[i]);
        ligne += historique[i].length() / 22 + 2;
        // info.print(0,ligne-1,"                    ",separateur[i]);
    }
}

void Jeu::InfoJeu(unsigned int attaquant, unsigned int cible, int sc)
{
    string nouvelleinfo;

    switch (sc)
    {
    case 2:
        nouvelleinfo = " " + nom[attaquant] + " vient de couler un navire de " + nom[cible];
        break;
    case 1:
        nouvelleinfo = " " + nom[attaquant] + " a touché " + nom[cible] + " en " + getPos(cible);

        break;
    }

    DecalerHistorique(nouvelleinfo, sc);
}

string Jeu::getPos(int i)
{
    string Alphabet = "0ABCDEFGHIJKLMNOPQRST";
    int *pos = (*Joueur[i]).getDernierTir();
    return Alphabet[pos[0] + 1] + myitoa(pos[1] + 1);
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
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W, 0 - (H + HF) / 2, 0 - 2 - sxf - 2 * WF, 4 + H + 0 - (H + HF) / 2, vitesse, difficulte, aide, estIA(),nom[0]);
        // plateau.print(sx+25+W/2,syf,nom[0].c_str());
        (*Joueur[0]).fenetre.printborder(0, 0, nom[0]);

        Joueur[1] = new Grille(0 + 2 + sx, 0 - (H + HF) / 2, 0 + 2 + sxf, 4 + H + 0 - (H + HF) / 2, vitesse, difficulte, aide, estIA(),nom[1]);
        break;
    }
    case 3 ... 4:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W, 0 + 1, 0 - 2 - sxf - 2 * WF, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(),nom[0]);
        Joueur[1] = new Grille(0 + 2 + sx, 0 + 1, 0 + 2 + sxf, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(),nom[1]);

        switch (nbjoueurs)
        {
        case 3:
            Joueur[2] = new Grille(0 - 2 - sx - 2 * W, 0 - H - 2, 0 - 2 - sxf - 2 * WF, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(),nom[2]);
            break;
        case 4:
            Joueur[2] = new Grille(0 - 2 - sx - 2 * W, 0 - H - 2, 0 - 2 - sxf - 2 * WF, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(),nom[2]);
            Joueur[3] = new Grille(0 + 2 + sx, 0 - H - 2, 0 + 2 + sxf, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(),nom[3]);
            break;
        }
        break;
    }
    case 5 ... 6:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W - 12, 0 + 1, 0 - 2 - sxf - 2 * WF - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(),nom[0]);
        Joueur[1] = new Grille(0 + 2 + sx - 12, 0 + 1, 0 + 2 + sxf - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(),nom[1]);
        Joueur[2] = new Grille(0 + 6 + sx + 2 * max(W, WF) - 12, 0 + 1, 0 + 6 + sxf + 2 * max(W, WF) - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(),nom[2]);

        Joueur[3] = new Grille(0 - 2 - sx - 2 * W - 12, 0 - H - 2, 0 - 2 - sxf - 2 * WF - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(),nom[3]);
        Joueur[4] = new Grille(0 + 2 + sx - 12, 0 - H - 2, 0 + 2 + sxf - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(),nom[4]);

        if (nbjoueurs == 6)
        {
            Joueur[5] = new Grille(0 + 6 + sx + 2 * max(W, WF) - 12, 0 - H - 2, 0 + 6 + sxf + 2 * max(W, WF) - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(),nom[5]);
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
    if (humains > 0)
    {
        humains--;

        return false;
    }
    else
    {

        return true;
    }
}

void Jeu::selectionCible(int j)
{

    if (nbjoueurs == 2)
    {
        switch (joueur)
        {
        case 0:
            selectRight();
            break;
        case 1:
            selectLeft();
            break;
        }

        (*Joueur[cibleSelectionnee]).setCiblageValide(true);
        return;
    }

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
                            (*Joueur[cibleSelectionnee]).setCiblageValide(true);
                return;
            break;
        }
        case '+':
            vitesse = min(vitesse + 1, 9);
            aide.print(aide.getWindowWidth() - 2, 0, myitoa(vitesse), BCYAN);
            break;
        case '-':
            vitesse = max(vitesse - 1, 0);
            aide.print(aide.getWindowWidth() - 2, 0, myitoa(vitesse), BCYAN);

            break;
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
    } while (cible == joueur || (*Joueur[cible]).estMort());
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
    usleep(min((9 - vitesse) * 22000, 333333));
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
        (*Joueur[cibleSelectionnee]).setEstCible(false);
bool trouve = false;
while (!trouve)
{
    if (cibleSelectionnee < nbjoueurs -1)
    {
        cibleSelectionnee++;
    }
    else
    {
        for (int i = 0 ; i < nbjoueurs ; i++)
        {
            if (!(Joueur[i]->estMort()))
            {
                trouve = true;
                cibleSelectionnee = i;
                break;
            }
        }
    }
    if (!(Joueur[cibleSelectionnee]->estMort()))
    {
        trouve = true;
    }
}


        (*Joueur[cibleSelectionnee]).setEstCible(true);
    
}

void Jeu::selectLeft()
{
{
        (*Joueur[cibleSelectionnee]).setEstCible(false);
bool trouve = false;
while (!trouve)
{
    if (cibleSelectionnee > 0)
    {
        cibleSelectionnee--;
    }
    else
    {
        for (int i = nbjoueurs-1 ; i >= 0 ; i--)
        {
            if (!(Joueur[i]->estMort()))
            {
                trouve = true;
                cibleSelectionnee = i;
                break;
            }
        }
    }
    if (!(Joueur[cibleSelectionnee]->estMort()))
    {
        trouve = true;
    }
}


        (*Joueur[cibleSelectionnee]).setEstCible(true);
    
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

    wattron(aide.win, A_BOLD);
    (*Joueur[0]).flotte.fenetre.setCarBordure(' ');
    (*Joueur[0]).flotte.fenetre.setCouleurBordure(GBLACK);
    info.setBordureDroite();
}

void Jeu::joueurSuivant()
{
    (*Joueur[cibleSelectionnee]).setEstCible(false);

    (*Joueur[joueur]).setJoue(false);

bool trouve = false;
for (int i = joueur+1 ; i < nbjoueurs  && !trouve; i++)
{
    if (!(*Joueur[joueur]).estMort())
    {
        trouve = true;
        joueur = i;
    }
}
if (!trouve)
{
    for (int i = 0 ; i < nbjoueurs && !trouve; i++ )
    {
    if (!(*Joueur[joueur]).estMort())
    {
        trouve = true;
        joueur = i;
    }
    }
}
    
    (*Joueur[joueur]).setJoue(true);
    cibleSelectionnee = joueur;
    usleep(min((9 - vitesse) * 20000, 0));
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
        return (-2 * max(W, WF) - 26 + max(0, WF - W) - 1);
    }
    else
    {
        return (-2 * max(W, WF) - 38 + max(0, WF - W) - 1);
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
    wattron(info.frame, A_UNDERLINE);
    wattron(info.frame, A_BOLD);
    wattron(info.frame, COLOR_PAIR(YBLACK));
    mvwprintw(info.frame, 0, 9, "SCORES", YBLACK);
    wattroff(info.frame, COLOR_PAIR(YBLACK));
    wattroff(info.frame, A_UNDERLINE);

    string t = "Tour " + myitoa(tour);

    wattron(info.frame, COLOR_PAIR(RBLACK));

    mvwprintw(info.frame, 23, 24 - t.length(), t.c_str());
    wattroff(info.frame, COLOR_PAIR(RBLACK));
    wattroff(info.frame, A_BOLD);

    for (int i = 0; i < nbjoueurs; i++)
    {
        string sc = myitoa((*Joueur[joueurEnPosition[i]]).Score.getScore());
        switch (sc.length())
        {
        case 4:
            sc.insert(1, " ");
            break;
        case 5:
            sc.insert(2, " ");
            break;
        case 6:
            sc.insert(3, " ");
            break;
        }

        string str = nom[i];
        string str2 = myitoa((*Joueur[joueurEnPosition[i]]).Score.getScore() / 1000) + " " + myitoa((*Joueur[joueurEnPosition[i]]).Score.getScore() % 1000);
        info.print(0, i, "                    ");
        info.print(0, i, nom[joueurEnPosition[i]]);
        info.print(info.getLargeur() - sc.length(), i, sc);
        // info.print(1,13+i,myitoa(i));
    }
}

void Jeu::classementDown()
{

    if (positionDuJoueur[joueur] < nbjoueurs - 1 && (*Joueur[joueur]).getScore() < (*Joueur[joueurEnPosition[positionDuJoueur[joueur] + 1]]).getScore())
    {
        int pos = joueurEnPosition[positionDuJoueur[joueur] + 1];

        swap(joueurEnPosition[positionDuJoueur[joueur]], joueurEnPosition[positionDuJoueur[joueur] + 1]);
        swap(positionDuJoueur[joueur], positionDuJoueur[pos]);

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

        cout << "test" << endl;
        cout << "test" << endl;
        cout << "test" << endl;
        cout << "test" << endl;
        classementDown();
    }
}

void Jeu::classementUp()
{

    if (positionDuJoueur[joueur] > 0 && (*Joueur[joueur]).getScore() > (*Joueur[joueurEnPosition[positionDuJoueur[joueur] - 1]]).getScore())
    {
        int pos = joueurEnPosition[positionDuJoueur[joueur] - 1];
        swap(joueurEnPosition[positionDuJoueur[joueur]], joueurEnPosition[positionDuJoueur[joueur] - 1]);

        swap(positionDuJoueur[joueur], positionDuJoueur[pos]);

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