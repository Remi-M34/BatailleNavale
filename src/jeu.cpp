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

Jeu::Jeu(int nbjoueurs, int humains, int diff, int v, string n[], string log, bool partiechargee) : nbjoueurs(nbjoueurs),
                                                                                                    humains(humains),
                                                                                                    info(22, 22, getScoreStartX(nbjoueurs) - 2, -(H + HF) / 1.9),
                                                                                                    plateau(getBordureHeight(nbjoueurs), getBordureWidth(nbjoueurs), getScoreStartX(nbjoueurs) - 4, getBordureStartY(nbjoueurs)),
                                                                                                    aide(7, 2 * W + 4, getScoreStartX(nbjoueurs) + 22, getAideStartY(nbjoueurs)),
                                                                                                    difficulte(diff),
                                                                                                    vitesse(v), JoueursRestants(nbjoueurs), fichierlog(log), partiechargee(partiechargee)

{

    stop = false;
    joueur = 0;
    cible = 0;
    cibleSelectionnee = 0;
    nbhisto = 0;
    tour = 0;

    for (int i = 0; i < 5; i++)
    {
        separateur[i] = WBLACK;
    }

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

Jeu::~Jeu()
{
    // for (int i = 0 ; i < nbjoueurs ; i++)
    // {
    //     delete[] Joueur[i];
    // }
    // delete[] Joueur;
}










void Jeu::Phase1()
{
    CreationLog();
    DecalerHistorique("Commandes:\n\nSélection     Entrée\nValider       Entrée\nPivoter D     Espace\nPivoter G          w\nAnnuler navire     b\nAléatoire          a", 0);
    DecalerHistorique("Les joueurs peuvent placer leurs navires sur leurs territoires!", 0);
    placementDesNavires();
    historique[1].clear();
    nbhisto--;
    DecalerHistorique("Tirage au sort pour\ndésigner le premier\njoueur...", 1);
    usleep(1500000);
    joueur = rand() % nbjoueurs;
    string str = nom[joueur] + " commence!";
    DecalerHistorique(str, 1);

    Phase2();
}

void Jeu::Phase2()
{

    setupaide();
    refreshScores();

    (*Joueur[joueur]).setJoue(true);
    usleep(150000);

    selectionCible();
    while (JoueursRestants > 1 && !stop)
    {

        switch (attaque())
        {
            //  = navire touché
        case 1:

            payback[cibleSelectionnee] = joueur;
            (*Joueur[joueur]).Score.navireTouche();
            InfoJeu(joueur, cibleSelectionnee, 1);
            Log_AjouterAction(1);
            classementUp();

            estVulnerable[cibleSelectionnee] = 1;
            continue;

        // 2 = navire coulé
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

        // 3 = navire coulé + joueur tué
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
                selectionCible();
            }
            continue;

        // 0 = missile tombé à l'eau
        case 0:
            (payback[cibleSelectionnee]) = joueur;

            (*Joueur[joueur]).Score.missileEnvoye();
            Log_AjouterAction(0);

            joueurSuivant();
            classementDown();
            usleep(max((9 - vitesse) * 27000, 0));
            selectionCible();
            break;
        
        // -1 = L'utilisateur a appuyé sur c pour changer de cible
        case -1:
            if (JoueursRestants > 2)
            {
                selectionCible();
            }
            else
            {
                DecalerHistorique("Impossible de changer de cible!", 0);
            }
            break;

        case -2:
            sauvegarde();
            if (JoueursRestants > 2)
            {
                selectionCible();
            }
            break;
        case -3:
            RetourMenu();
            stop = true;
            break;

        // -4 = l'utilisateur a appuyé sur n pour afficher les navires
        case -4:
            navires();
            break;
        }
    }

    if (JoueursRestants == 1)
    {
        ValiderScore();
    }
    plateau.clearall();
    erase();
}




void Jeu::CreationLog()
{
    //Creer un fichier log et écrit la configuration de la parie
    //Si un fichier log existe déja, il écrase celui-ci puis écrit la configuration de la parie
    string str = "logs/" + fichierlog;
    std::ofstream file(str.c_str());
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
    string str = "logs/" + fichierlog;
    std::ofstream file(str.c_str(), std::ios_base::app);
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

void Jeu::chargementparametres(int j, int jrestants, int tour, int nbhisto, int payback[6], int estVulnerable[6], int positionDuJoueur[6], int joueurEnPosition[6], string historique[6], int ***Case, int ***Case2, int **casesRestantes, int ***posNavires, int *naviresRestants, int **NbPivotements, int *MissilesTires, int *MissilesGagnants, int tailleFlotte)
{
    this->nbhisto = nbhisto;
    this->tour = tour;
    this->JoueursRestants = jrestants;
    this->joueur = j;
    for (int i = 0; i < 6; i++)
    {
        this->payback[i] = payback[i];
        this->estVulnerable[i] = estVulnerable[i];
        this->positionDuJoueur[i] = positionDuJoueur[i];
        this->joueurEnPosition[i] = joueurEnPosition[i];
        this->historique[i] = historique[i];

        if (i < nbjoueurs)
        {
            Joueur[i]->ChargementDonnees(Case[i], Case2[i], casesRestantes[i], posNavires[i], naviresRestants[i], NbPivotements[i], MissilesTires[i], MissilesGagnants[i], tailleFlotte);
        }
    }

    Joueur[0]->flotte.fenetre.clearall();

    setupaide();
    DecalerHistorique(" Partie chargée!", 0);
}

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
                plateau.setBordureFine();
            }
            break;
        case 25:
            if (ligne[0] == 'O' || ligne[0] == 'o')
            {
                info.setBordureFine();
            }
            break;
        }
        lignes++;
    }

    couleurs.close();
}

void Jeu::setupaide()
{

    aide.setBordureFine();

    aide.print(1, 0, "Vitesse ( +, - )", CBLACK);
    aide.print(aide.getWindowWidth() - 2, 0, myitoa(vitesse), CBLACK);
    aide.print(1, 1, "Sauvegarder", CBLACK);
    aide.print(aide.getWindowWidth() - 2, 1, 's', CBLACK);
    aide.print(1, 2, "Menu", CBLACK);
    aide.print(aide.getWindowWidth() - 2, 2, 'm', CBLACK);
    if (humains == 0)
    {
        aide.print(0, 4, "Mode spectateur.\nRéglez la vitesse du jeu avec les touches + et -.", WBLACK);
    }
    else
    {
        aide.print(1, 1, "Revoir navires", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 1, "n");

        aide.print(1, 2, "Annuler cible", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 2, "c");

        aide.print(1, 3, "Case préc/suivante", CBLACK);
        aide.print(aide.getWindowWidth() - 4, 3, "z/e");

        aide.print(1, 4, "Tir aléatoire", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 4, "a");
        aide.print(1, 5, "Sauvegarder", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 5, "s");
        aide.print(1, 6, "Menu", CBLACK);
        aide.print(aide.getWindowWidth() - 2, 6, "m");
    }
}

void Jeu::RetourMenu()
{
    stop = true;
    Window warning(6, 30, -18, -2);
    warning.setCouleurBordure(BRED);
    warning.setBordureFine();
    warning.print(1, 1, "Sauvegarder la partie ?", BLUEBLACK);
    int ch;
    Color col[2] = {WBLUE, BBLUE};
    while (ch = getch())
    {
        warning.print(7, 4, "Oui", col[0]);
        warning.print(20, 4, "Non", col[1]);
        switch (ch)
        {
        case KEY_RIGHT:
            swap(col[0], col[1]);
            break;
        case KEY_LEFT:
            swap(col[0], col[1]);
            break;
        case '\n':
            if (col[0] == WBLUE)
            {
                sauvegarde();
                usleep(1500000);
            }
            return;
        }
    }
}



void Jeu::DecalerHistorique(string nouvelleinfo, int sc)
{
    info.setBordureFine();

    for (int i = 5; i > 0; i--)
    {
        historique[i] = historique[i - 1];
        separateur[i] = separateur[i - 1];
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
    }

    if (tour > 1)
    {
        refreshScores();
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
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W, 0 - (H + HF) / 2, 0 - 2 - sxf - 2 * WF, 4 + H + 0 - (H + HF) / 2, vitesse, difficulte, aide, estIA(), nom[0], partiechargee);
        // plateau.print(sx+25+W/2,syf,nom[0].c_str());
        (*Joueur[0]).fenetre.printborder(0, 0, nom[0]);

        Joueur[1] = new Grille(0 + 2 + sx, 0 - (H + HF) / 2, 0 + 2 + sxf, 4 + H + 0 - (H + HF) / 2, vitesse, difficulte, aide, estIA(), nom[1], partiechargee);
        break;
    }
    case 3 ... 4:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W, 0 + 1, 0 - 2 - sxf - 2 * WF, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[0], partiechargee);
        Joueur[1] = new Grille(0 + 2 + sx, 0 + 1, 0 + 2 + sxf, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[1], partiechargee);

        switch (nbjoueurs)
        {
        case 3:
            Joueur[2] = new Grille(0 - 2 - sx - 2 * W, 0 - H - 2, 0 - 2 - sxf - 2 * WF, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[2], partiechargee);
            break;
        case 4:
            Joueur[2] = new Grille(0 - 2 - sx - 2 * W, 0 - H - 2, 0 - 2 - sxf - 2 * WF, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[2], partiechargee);
            Joueur[3] = new Grille(0 + 2 + sx, 0 - H - 2, 0 + 2 + sxf, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[3], partiechargee);
            break;
        }
        break;
    }
    case 5 ... 6:
    {
        Joueur[0] = new Grille(0 - 2 - sx - 2 * W - 12, 0 + 1, 0 - 2 - sxf - 2 * WF - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[0], partiechargee);
        Joueur[1] = new Grille(0 + 2 + sx - 12, 0 + 1, 0 + 2 + sxf - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[1], partiechargee);
        Joueur[2] = new Grille(0 + 6 + sx + 2 * max(W, WF) - 12, 0 + 1, 0 + 6 + sxf + 2 * max(W, WF) - 12, 4 + H + 0 + 1, vitesse, difficulte, aide, estIA(), nom[2], partiechargee);

        Joueur[3] = new Grille(0 - 2 - sx - 2 * W - 12, 0 - H - 2, 0 - 2 - sxf - 2 * WF - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[3], partiechargee);
        Joueur[4] = new Grille(0 + 2 + sx - 12, 0 - H - 2, 0 + 2 + sxf - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[4], partiechargee);

        if (nbjoueurs == 6)
        {
            Joueur[5] = new Grille(0 + 6 + sx + 2 * max(W, WF) - 12, 0 - H - 2, 0 + 6 + sxf + 2 * max(W, WF) - 12, 0 - 6 - H - HF, vitesse, difficulte, aide, estIA(), nom[5], partiechargee);
        }
        break;
    }
    }

    for (int i = 0; i < nbjoueurs; i++)
    {
        payback[i] = rand() % nbjoueurs;
        estVulnerable[i] = 0;
        positionDuJoueur[i] = i;
        joueurEnPosition[i] = i;
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

void Jeu::selectionCible()
{

    if (JoueursRestants == 2)
    {
        do
        {
            selectRight();
        } while (cibleSelectionnee == joueur);
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
        case 'm':
            RetourMenu();
            return;
            break;
        case '\n':
            if (joueur != cibleSelectionnee && !Joueur[cibleSelectionnee]->estMort())
            {
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
        (*Joueur[cibleSelectionnee]).setEstCible(false);

        cibleSelectionnee = j;
        (*Joueur[cibleSelectionnee]).setEstCible(true);
    }
}

void Jeu::joueurVulnerable()
{
    int k = 0;
    int *vul = new int[JoueursRestants];

    for (int i = 0; i < nbjoueurs; i++)
    {
        vul[i] = -1;
        if (!(Joueur[i]->estMort()) && (estVulnerable[i] == 1) && joueur != i)
        {
            vul[k] = i;
            k++;
        }
    }
    if (k == 0)
    {
        cibleAleatoire();
        delete[] vul;
        return;
    }
    cible = vul[(rand() % k)];
    delete[] vul;

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
            switch (rand() % 3)
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

    } while (cible == joueur);
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

            {
                selectUp();
            }
            else
            {
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
        string str = nom[i] + "\nplace ses navires...";
        info.print(0, 0, str);
        Joueur[i]->selectionNavire();
    }

    wattron(aide.win, A_BOLD);
    (*Joueur[0]).flotte.fenetre.setCarBordure(' ');
    (*Joueur[0]).flotte.fenetre.setCouleurBordure(GBLACK);
    info.setBordureFine();
}

void Jeu::joueurSuivant()
{
    tour++;

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
    if (nbjoueurs < 5)
    {
        return 36 + 4 * max(W, WF) - max(0, WF - W);
    }
    else
    {
        return 40 + 6 * max(W, WF) - max(0, WF - W);
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

        Joueur[i]->fenetre.clear();
    }
    Flotte nav(Joueur[1]->flotte.fenetre.getX() - COLS / 2, Joueur[1]->flotte.fenetre.getY() - LINES / 2, 0, false);
    getchar();

    Joueur[1]->flotte.fenetre.setCouleurBordure(Joueur[1]->flotte.fenetre.getCouleurBordure());

    for (int i = 0; i < nbjoueurs; i++)
    {

        if (i == cibleSelectionnee)
        {
            (*Joueur[i]).setEstCible(true);
        }
        else
            (*Joueur[i]).setEstCible(false);
    }
    for (int i = 0; i < nbjoueurs; i++)
    {
        (*Joueur[i]).refreshGrille(0, 0, W, H);
    }
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
    }
}

void Jeu::classementDown()
{

    if (positionDuJoueur[joueur] < nbjoueurs - 1 && (*Joueur[joueur]).getScore() < (*Joueur[joueurEnPosition[positionDuJoueur[joueur] + 1]]).getScore())
    {
        int pos = joueurEnPosition[positionDuJoueur[joueur] + 1];

        swap(joueurEnPosition[positionDuJoueur[joueur]], joueurEnPosition[positionDuJoueur[joueur] + 1]);
        swap(positionDuJoueur[joueur], positionDuJoueur[pos]);

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

        classementUp();
    }

    refreshScores();
}

void Jeu::ValiderScore()
{

    Window endgame(14, 60, -35, -6);
    endgame.setCouleurBordure(BRED);
    endgame.setBordureFine();
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

    // fstream configcopy("save/config.txt", ios::in | ios::out | ios::trunc);
    // ifstream config("config/config.txt", ios::in);
    ifstream src("config/config.txt", ios::binary);
    ofstream dest("save/config.txt", ios::binary);
    dest << src.rdbuf();
    src.close();
    dest.close();

    string str = "logs/" + fichierlog;
    ifstream srclog(str.c_str(), ios::binary);
    string cheminlog = "save/" + fichierlog;
    ofstream destlog(cheminlog.c_str(), ios::binary);
    destlog << srclog.rdbuf();
    srclog.close();
    destlog.close();

    fstream save("save/sauvegarde", ios::in | ios::out | ios::trunc);
    save << fichierlog << endl
         << humains << ':'
         << nbjoueurs << ':'
         << joueur << ':'
         << JoueursRestants << ':'
         << '0' << ':'
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

    DecalerHistorique(" Partie sauvegardée avec succès !", 0);
    refreshScores();
    save.close();
}
