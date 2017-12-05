#ifndef __JEU_H
#define __JEU_H

// #include "window.h"
// #include "config.h"
// #include "grille.h"
// extern "C" {
// #include <curses.h>
// }
// #include <string>



class Jeu{

public:

  Jeu(int nbjoueurs, int humains, int difficulte, int v);

  //destructeur
  ~Jeu();
// Démarre le jeu
void  start();
// Phase où chacun des joueurs placent leur navires.
void  placementDesNavires();

void initCouleurs();
// Initialise toutes les grilles dont les positions dépendent du nombre de joueurs (afin de toujours centrer le jeu)
void initDim(int j);
bool estIA();
// Fonctions permettant la sélection de cible
void selectionCible(int c);
void selectUp();
void selectDown();
void selectRight();
void selectLeft();
// Détermine à qui le tour
void joueurSuivant();
int attaque();
void selectionCibleAleatoire();
void deplacementIA();
void navires();
// Renvoie un joueur "vulnérable" cad ayant au moins un navire touché (mais non coulé).
// Est plus ou moins pris en compte par l'IA selon le mode de difficulté choisie
int joueurVulnerable();
// Renvoie aléatoirement un joueur qui sera la cible du joueur en train de jouer (sauf lui-même)
void cibleAleatoire();

private:
      Window score;
      Window aide;
      Window plateau;
      int humains;
      int nbjoueurs;
      int vitesse = 3;
      int joueur = 0;
      int cible;
      int cibleSelectionnee = 0;
      // Gère les affinités, utilisé par l'IA.
      // Selon le mode de difficulté, renvoie et cible le dernier joueur ayant attaqué avec succès le joueur en train de jouer.
      int *payback;
      int *estVulnerable;
       int const difficulte;

      
      Grille *Joueur[];

      int scoreWidth;
      int syf2;
      int sxf2;
      int sy2, sx2;

      int scoreJoueur1 = 0;
      int scoreJoueur2 = 0;

  // Options du thème
      Color colScore;
      Color colAide;
      Color colBordureGrilles;
      Color colBordurePlateau;


     int sx,sy,sxf,syf;

};

int getScoreStartX(int t);
int getBordureWidth(int nbjoueurs);
int getBordureHeight(int nbjoueurs);
int getBordureStartY(int nbjoueurs);
int getAideStartY(int nbjoueurs);


void checkSpeed(int ch, Window &aide, int &vitesse);

#endif

