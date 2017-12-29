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

Jeu::Jeu(int n) : info(22, 22, getScoreStartX(2) - 2, -(H + HF) / 1.9), plateau(getBordureHeight(2), getBordureWidth(2), getScoreStartX(2) - 4, getBordureStartY(2)), aide(7, 2 * W + 4, getScoreStartX(2) + 22, getAideStartY(2)), difficulte(2)
{
    srand((int)time(0));

    nbjoueurs = 2;
    humains = (2);
    vitesse = (6);
    JoueursRestants = (2);

    initCouleurs();
    curs_set(0);

    initDim(2);
}

void Jeu::CreationLog()
{
    //Creer un fichier "fichier.log" et écrit la configuration de la parie
    //Si un fichier "fichier.log" existe déja, il écrase celui-ci puis écrit la configuration de la parie
    std::ofstream file("fichier.log");
    file << "Début d'une nouvelle partie à: " << nbjoueurs << " joueurs\n";
    file << "La taille de la grille de jeu est de: " << W << "x" << H << "\n";
    file << "Difficulté: " << (difficulte == 1 ? "Facile" : (difficulte == 2 ? "Normal" : "Difficile")) << "\n";
    file << "Participants:\n";
    for (int i = 0; i < nbjoueurs; i++)
    {
        file << "\t" << nom[i] << "\n";
    }
    file << " \n";
}

void Jeu::Log_AjouterAction(int sc)
{
    //Permet d'ajouter les actions tour par tour
    std::ofstream file("fichier.log", std::ios_base::app);
    file << "Tour " << tour << "\n";
    file << nom[joueur] << " cible la flotte de " << nom[cibleSelectionnee] << "\n";
    file << "\tun missile est tiré sur la case " << getPos(cibleSelectionnee) << "\n";
    switch (sc)
    {
    case 1:

        file << "\tle missile a touché la cible !\n";
        break;
    case 2:
        file << "\tle missile a coulé un navire de la cible !\n";
        break;
    case 3:
        file << "\tle missile a coulé un navire de la cible !\n";
        file << nom[cibleSelectionnee] << " repose en paix...\n";
        break;
    case 0:

        file << "\tle missile est tombé à l'eau...\n";
        break;
    }
    file << "\tle score de " << nom[joueur] << " est de " << myitoa(Joueur[joueur]->Score.getScore() / 1000);

    if (Joueur[joueur]->Score.getScore() > 999)
    {
        file << " " << (Joueur[joueur]->Score.getScore() % 1000 < 100 && Joueur[joueur]->Score.getScore() % 1000 > 0 ? "0" : "") << Joueur[joueur]->Score.getScore() % 1000 << ((*Joueur[joueur]).Score.getScore() % 1000 == 0 ? "00" : "");
    }

    file << " Points" << endl
         << endl
         << endl;
}

void Jeu::chargementparametres(int tour, int nbhisto, int payback[6], int estVulnerable[6], int positionDuJoueur[6], int joueurEnPosition[6], string historique[6], int ***Case, int ***Case2, int **casesRestantes, int ***posNavires,int* naviresRestants, int** NbPivotements, int *MissilesTires, int *MissilesGagnants, int tailleFlotte)
{
    this->nbhisto = nbhisto;
    this->tour = tour;
    for (int i = 0; i < 6; i++)
    {
        this->payback[i] = payback[i];
        this->estVulnerable[i] = estVulnerable[i];
        this->positionDuJoueur[i] = positionDuJoueur[i];
        this->joueurEnPosition[i] = joueurEnPosition[i];
        this->historique[i] = historique[i];

        if (i < nbjoueurs)
        {
            Joueur[i]->ChargementDonnees(Case[i], Case2[i], casesRestantes[i], posNavires[i], naviresRestants[i], NbPivotements[i],MissilesTires[i], MissilesGagnants[i], tailleFlotte);
        }
    }

    setupaide();
    DecalerHistorique(" Partie chargée!", 0);
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
}

void Jeu::Phase2()
{

    setupaide();
    refreshScores();

    (*Joueur[joueur]).setJoue(true);
    usleep(150000);

    selectionCible(joueur);
    int ch;

    while (JoueursRestants > 1)
    {
        tour++;
        // checkSpeed(ch,aide,vitesse);

        switch (attaque())
        {
        case 1:

            payback[cibleSelectionnee] = joueur;
            (*Joueur[joueur]).Score.navireTouche();
            InfoJeu(joueur, cibleSelectionnee, 1);
            Log_AjouterAction(1);
            classementUp();

            estVulnerable[cibleSelectionnee] = 1;
            continue;

        case 2:

            (payback[cibleSelectionnee]) = joueur;
            (*Joueur[joueur]).Score.navireTouche();
            InfoJeu(joueur, cibleSelectionnee, 2);
            Log_AjouterAction(2);
            classementUp();
            if ((*Joueur[cibleSelectionnee]).findFocus() == -1)
            {
                estVulnerable[cibleSelectionnee] = 0;
            }
            continue;
        case 3:
            JoueursRestants--;

            Joueur[joueur]->Score.navireTouche();
            Log_AjouterAction(3);

            classementUp();
            InfoJeu(joueur, cibleSelectionnee, 3);
            classementUp();
            Joueur[cibleSelectionnee]->croixmort();
            if (JoueursRestants > 1)
            {
                selectionCible(cibleSelectionnee);
            }
            continue;

        case 0:
            (payback[cibleSelectionnee]) = joueur;

            (*Joueur[joueur]).Score.missileEnvoye();
            Log_AjouterAction(0);

            joueurSuivant();
            classementDown();
            usleep(max((9 - vitesse) * 27000, 0));
            selectionCible(joueur);

            break;
        case -1:
            (*Joueur[cibleSelectionnee]).setCiblageValide(false);
            tour--;
            if (JoueursRestants > 2)
            {
                selectionCible(joueur);
            }
            break;
        }
    }

    ValiderScore();

    plateau.clearall();
}

void Jeu::Phase1()
{
    CreationLog();
    DecalerHistorique(" Les joueurs peuvent \ndésormais placer leur navires sur leur territoire!", 0);
    placementDesNavires();
    DecalerHistorique(" Le jeu commence !", 0);

    Phase2();
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
    case 3:
        separateur[0] = RBLACK;
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
    case 3:
        nouvelleinfo = " " + nom[attaquant] + " a tué " + nom[cible] + " !";
        break;
    case 2:
        nouvelleinfo = " " + nom[attaquant] + " vient de couler un navire de " + nom[cible];
        break;
    case 1:
        nouvelleinfo = " " + nom[attaquant] + " a touché " + nom[cible] + " en " + getPos(cible);
        break;
    }
    DecalerHistorique(nouvelleinfo, sc);

    if (JoueursRestants == 1)
    {
        string sco = myitoa(Joueur[attaquant]->Score.getScore() / 1000) + " " + (Joueur[attaquant]->Score.getScore() % 1000 < 100 && Joueur[attaquant]->Score.getScore() % 1000 > 0 ? "0" : "") + myitoa(Joueur[attaquant]->Score.getScore() % 1000) + ((*Joueur[attaquant]).Score.getScore() % 1000 == 0 ? "00" : "");

        nouvelleinfo = nom[attaquant] + " est déclaré vainqueur avec " + sco + " points !";

        DecalerHistorique(nouvelleinfo, sc);
    }
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
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W, 0 - (H + HF) / 2, 0 - 2 - sxf - 2 * WF, 4 + H + 0 - (H + HF) / 2, vitesse, difficulte, aide, estIA(), nom[0]);
        // plateau.print(sx+25+W/2,syf,nom[0].c_str());
        (*Joueur[0]).fenetre.printborder(0, 0, nom[0]);

        Joueur[1] = new Grille(0 + 2 + sx, 0 - (H + HF) / 2, 0 + 2 + sxf, 4 + H + 0 - (H + HF) / 2, vitesse, difficulte, aide, estIA(), nom[1]);
        break;
    }
    case 3 ... 4:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W, 0 + 1, 0 - 2 - sxf - 2 * WF, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[0]);
        Joueur[1] = new Grille(0 + 2 + sx, 0 + 1, 0 + 2 + sxf, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[1]);

        switch (nbjoueurs)
        {
        case 3:
            Joueur[2] = new Grille(0 - 2 - sx - 2 * W, 0 - H - 2, 0 - 2 - sxf - 2 * WF, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[2]);
            break;
        case 4:
            Joueur[2] = new Grille(0 - 2 - sx - 2 * W, 0 - H - 2, 0 - 2 - sxf - 2 * WF, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[2]);
            Joueur[3] = new Grille(0 + 2 + sx, 0 - H - 2, 0 + 2 + sxf, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[3]);
            break;
        }
        break;
    }
    case 5 ... 6:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W - 12, 0 + 1, 0 - 2 - sxf - 2 * WF - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[0]);
        Joueur[1] = new Grille(0 + 2 + sx - 12, 0 + 1, 0 + 2 + sxf - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[1]);
        Joueur[2] = new Grille(0 + 6 + sx + 2 * max(W, WF) - 12, 0 + 1, 0 + 6 + sxf + 2 * max(W, WF) - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[2]);

        Joueur[3] = new Grille(0 - 2 - sx - 2 * W - 12, 0 - H - 2, 0 - 2 - sxf - 2 * WF - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[3]);
        Joueur[4] = new Grille(0 + 2 + sx - 12, 0 - H - 2, 0 + 2 + sxf - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[4]);

        if (nbjoueurs == 6)
        {
            Joueur[5] = new Grille(0 + 6 + sx + 2 * max(W, WF) - 12, 0 - H - 2, 0 + 6 + sxf + 2 * max(W, WF) - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[5]);
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

    if (JoueursRestants == 2)
    {
        do
        {
            selectRight();
        } while (cibleSelectionnee == joueur);
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
        case '1':
            if (!Joueur[0]->estMort() && joueur != 0)
            {
                SelectionKeypad(0);
                return;
            }
            break;
        case '2':
            if (!Joueur[1]->estMort() && joueur != 1)
            {
                SelectionKeypad(1);
                return;
            }
            break;
        case '3':
            if (nbjoueurs > 2 && !Joueur[2]->estMort() && joueur != 2)
            {
                SelectionKeypad(2);
                return;
            }
            break;
        case '4':
            if (nbjoueurs > 3 && !Joueur[3]->estMort() && joueur != 3)
            {
                SelectionKeypad(3);
                return;
            }
            break;
        case '5':
            if (nbjoueurs > 4 && !Joueur[4]->estMort() && joueur != 4)
            {
                SelectionKeypad(4);
                return;
            }
            break;
        case '6':
            if (nbjoueurs > 5 && !Joueur[5]->estMort() && joueur != 5)
            {
                SelectionKeypad(5);
                return;
            }
            break;
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
        case 's':
            sauvegarde();
            break;
        case '\n':
            if (joueur != cibleSelectionnee && !Joueur[cibleSelectionnee]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setCiblageValide(true);
                return;
            }
            else
            {
                (*Joueur[cibleSelectionnee]).mauvaiseSelection();
            }

            break;
        }
    }
}

void Jeu::SelectionKeypad(int j)
{
    if (nbjoueurs > j && !Joueur[j]->estMort() && j != joueur)
    {
        (*Joueur[cibleSelectionnee]).setCiblageValide(false);
        (*Joueur[cibleSelectionnee]).setEstCible(false);

        cibleSelectionnee = j;
        (*Joueur[cibleSelectionnee]).setEstCible(true);

        Joueur[cibleSelectionnee]->setCiblageValide(true);
    }
}

void Jeu::joueurVulnerable()
{
    int k = 0;
    int vul[JoueursRestants] = {-1};
    // srand((int)time(0));

    for (int i = 0; i < nbjoueurs; i++)
    {
        test("\n Rech joueur vul");
        if (!(Joueur[i]->estMort()) && (estVulnerable[i] == 1) && joueur != i)
        {
            vul[k] = i;
            k++;
        }
    }
    if (k == 0)
    {
        cibleAleatoire();
        return;
    }

    cible = vul[(rand() % k)];
    test(myitoa(cible));
    test(myitoa(k));
    test(myitoa(vul[k]));

    return;
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

            joueurVulnerable();
            break;
        case 2:
            if ((rand() % 2) == 0)
            {
                joueurVulnerable();
            }
            else
            {
                if (!Joueur[payback[joueur]]->estMort())
                {
                    cible = payback[joueur];
                }
                else
                    cibleAleatoire();
            }
            break;
        case 1:
            switch (int k = (rand() % 3))
            {
            case 0:
                joueurVulnerable();
                break;
            case 1:
            {
                if (!Joueur[payback[joueur]]->estMort())
                {
                    cible = payback[joueur];
                }
                else
                    cibleAleatoire();
            }
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
                test("left");

                selectLeft();
            }
            else
            {
                test("right");

                selectRight();
            }
            break;
        case 3 ... 5:
        {
            if (nbjoueurs > 4)

            {
                test("up");

                selectUp();
            }
            else
            {
                test("right");

                selectRight();
            }
        }
        break;
        }
        break;
    case 3 ... 5:

        switch (cible)
        {
        case 0 ... 2:
            test("down");

            selectDown();
            break;
        case 3 ... 5:
            if (cibleSelectionnee > cible)
            {
                test("left");
                selectLeft();
            }
            else
            {
                test("right");

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
            if (!Joueur[3]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[3]).setEstCible(true);
                cibleSelectionnee = 3;
                return;
            }
            else
            {
                return selectLeft();
            }
        case 1:
            if (nbjoueurs > 4 && !Joueur[4]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[4]).setEstCible(true);
                cibleSelectionnee = 4;
                return;
            }
            else
            {
                return selectLeft();
            }
            return;
        case 2:
            if (nbjoueurs == 6 && !Joueur[5]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[5]).setEstCible(true);
                cibleSelectionnee = 5;
            }
            else
            {
                return selectRight();
            }
            return;
        }
    }
    else if (nbjoueurs <= 4)
    {
        switch (cibleSelectionnee)
        {
        case 0:
            if (nbjoueurs > 2 && !Joueur[2]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[2]).setEstCible(true);
                cibleSelectionnee = 2;
                return;
            }
            else
            {
                return selectRight();
            }
        case 1:
            if (nbjoueurs == 4 && !Joueur[3]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[3]).setEstCible(true);
                cibleSelectionnee = 3;
            }
            else
            {
                return selectRight();
            }
            return;
        }
    }
    else
    {
        // selectLeft();
    }
}

void Jeu::selectDown()
{
    if (nbjoueurs > 4)
    {
        switch (cibleSelectionnee)
        {
        case 3:
        {
            if (!Joueur[0]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[0]).setEstCible(true);
                cibleSelectionnee = 0;
            }
            else
            {
                return selectLeft();
            }
            break;
        }
        case 4:
            if (nbjoueurs > 4 && !Joueur[1]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[1]).setEstCible(true);
                cibleSelectionnee = 1;
            }
            else
            {
                return selectRight();
            }
            break;
        case 5:
            if (nbjoueurs == 6 && !Joueur[2]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[2]).setEstCible(true);
                cibleSelectionnee = 2;
            }
            else
            {
                return selectRight();
            }
            break;
        }
    }
    else if (nbjoueurs <= 4)
    {
        switch (cibleSelectionnee)
        {
        case 3:
            if (!Joueur[1]->estMort())
            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[1]).setEstCible(true);
                cibleSelectionnee = 1;
            }
            else
            {
                return selectRight();
            }
            break;
        case 2:
        {
            if (!Joueur[0]->estMort())

            {
                (*Joueur[cibleSelectionnee]).setEstCible(false);
                (*Joueur[0]).setEstCible(true);
                cibleSelectionnee = 0;
            }
            else
            {
                return selectLeft();
            }
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
        if (cibleSelectionnee < nbjoueurs - 1)
        {
            cibleSelectionnee++;
        }
        else
        {
            for (int i = 0; i < nbjoueurs; i++)
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
                for (int i = nbjoueurs - 1; i >= 0; i--)
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
        string str = nom[i] + " place ses\n navires...";
        info.print(0, 0, str);
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
    while (!trouve)
    {
        if (joueur < nbjoueurs - 1)
        {
            joueur++;
        }
        else
        {
            for (int i = 0; i < nbjoueurs; i++)
            {
                if (!(Joueur[i]->estMort()))
                {
                    trouve = true;
                    joueur = i;
                    break;
                }
            }
        }
        if (!(Joueur[joueur]->estMort()))
        {
            trouve = true;
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
        Color col = BLUEBLACK;
        string str2 = myitoa((*Joueur[joueurEnPosition[i]]).Score.getScore() / 1000) + " " + ((*Joueur[joueurEnPosition[i]]).Score.getScore() % 1000 < 100 && (*Joueur[joueurEnPosition[i]]).Score.getScore() % 1000 > 0 ? "0" : "") + myitoa((*Joueur[joueurEnPosition[i]]).Score.getScore() % 1000) + (Joueur[joueurEnPosition[i]]->Score.getScore() % 1000 == 0 ? "00" : "");
        info.print(0, i, "                    ");
        if (Joueur[joueurEnPosition[i]]->estMort())
        {
            col = RBLACK;
        }
        else if (i == positionDuJoueur[joueur])
        {
            col = GBLACK;
        }
        info.print(0, i, nom[joueurEnPosition[i]], col);
        info.print(info.getLargeur() - sc.length(), i, sc);
        col = WBLACK;

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

    refreshScores();
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

    refreshScores();
}

void Jeu::ValiderScore()
{
    Window endgame(14, 60, -35, -6);
    endgame.setCouleurBordure(BRED);
    endgame.setBordureDroite();
    endgame.print(1, 1, historique[0], BLUEBLACK);
    endgame.print(1, 4, "Votre pseudo a peut être été ajouté aux Top Scores...\n Vérifiez depuis le menu!");
    endgame.print(1, 8, "Appuyez sur Entrée pour revenir au menu.");
    endgame.print(27, 12, "Retour", WMAGENTA);

    fstream top("topscores", ios::in | ios::out);

    string ligne;
    int pos = 0;

    int count = 0;
    int k;

    while (getline(top, ligne))
    {
        if (count % 2 == 1)
        {
            k = atoi(ligne.c_str());
            if (k < Joueur[joueur]->Score.getScore())
            {
                break;
            }
            else
            {
                pos++;
            }
        }
        count++;
    }

    top.clear();
    top.seekg(0, ios::beg);

    fstream toptmp("topscores2", ios::in | ios::out | ios::trunc);

    while (pos > 0)
    {
        getline(top, ligne);
        toptmp << ligne << "\n";
        getline(top, ligne);
        toptmp << ligne << "\n";
        pos--;
    }

    toptmp << nom[joueur] << "\n"
           << Joueur[joueur]->Score.getScore() << "\n";

    while (getline(top, ligne))
    {
        toptmp << ligne << "\n";
    }
    rename("topscores2", "topscores");

    top.close();
    toptmp.close();

    int car;
    while ((car = getch()) != '\n')
    {
    }
}

void Jeu::sauvegarde()
{
    string ligne;

    fstream configcopy("config/config_save.txt", ios::in | ios::out | ios::trunc);
    ifstream config("config/config.txt", ios::in);

    while ((getline(config, ligne)))
    {
        configcopy << ligne << endl;
    }

    fstream save("save/sauvegarde", ios::in | ios::out | ios::trunc);
    save << humains << ':'
         << nbjoueurs << ':'
         << joueur << ':'
         << JoueursRestants << ':'
         << cible << ':'
         << cibleSelectionnee << ':'
         << min(nbhisto, 6) << ':'
         << difficulte << endl
         << tour << endl;
    for (int i = 0; i < 6; i++)
    {
        save << payback[i];
    }
    save << endl;
    for (int i = 0; i < 6; i++)
    {
        save << estVulnerable[i];
    }
    save << endl;
    for (int i = 0; i < nbjoueurs; i++)
    {
        save << positionDuJoueur[i];
    }
    save << endl;
    for (int i = 0; i < nbjoueurs; i++)
    {
        save << joueurEnPosition[i];
    }
    save << endl;
    for (int i = 0; i < nbjoueurs; i++)
    {
        save << Joueur[i]->getNaviresRestants() << ':';
    }
    save << endl;

    for (int i = 0; i < nbjoueurs; i++)
    {
        Joueur[i]->getCasesRestantes(save);
        save << endl;
    }

    for (int i = 0; i < nbjoueurs; i++)
    {
        Joueur[i]->getPosNavires(save);
        save << endl;
    }

    for (int i = 0; i < nbjoueurs; i++)
    {
        for (int n = 0; n < 5; n++)
        {
            save << Joueur[i]->getNbPivotements(n) << ':';        
            }
        save << endl;

    }

    for (int i = 0; i < nbjoueurs; i++)
    {
        save << nom[i] << endl;
    }
    for (int i = 0; i < min(nbhisto, 6); i++)
    {
        save << endl
             << historique[i];
        if (i != min(nbhisto, 6))
            save << endl
                 << "::";
    }

    save << endl
         << "SCORES" << endl;

    save << myitoa(Joueur[0]->Score.getTailleFlotte()) << endl
         << endl;

    for (int i = 0; i < nbjoueurs; i++)
    {
        save << Joueur[i]->Score.getMissilesTires() << endl
             << Joueur[i]->Score.getMissilesGagnants() << endl;
    }

    save << "GRILLES" << endl;

    for (int i = 0; i < nbjoueurs; i++)
    {
        Joueur[i]->SauvegarderGrille(save);
        save << endl;
    }
}
