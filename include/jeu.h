#ifndef __JEU_H
#define __JEU_H

#include "../include/window.h"
#include "../include/grille.h"

class Jeu
{

public:
  Jeu(int nbjoueurs, int humains, int difficulte, int v, std::string n[], std::string log, bool partiechargee);

  //destructeur
  ~Jeu();
  void initCouleurs();

  // Démarre le jeu - placement des navires, creation du log etc...
  void Phase1();
  void Phase2();
  // Phase où chacun des joueurs placent leur navires.
  void placementDesNavires();

  void InfoJeu(unsigned int attaquant, unsigned int cible, int sc);
  // Initialise toutes les grilles dont les positions dépendent du nombre de joueurs (afin de toujours centrer le jeu)
  void initDim(int j);
  bool estIA();
  // Fonctions permettant la sélection de cible
  void selectionCible();
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
  void joueurVulnerable();
  // Renvoie aléatoirement un joueur qui sera la cible du joueur en train de jouer (sauf lui-même)
  void cibleAleatoire();
  void refreshScores();
  void classementDown();
  void classementUp();
  void SelectionKeypad(int j);
  void DecalerHistorique(std::string nouvelleinfo, int sc);
  std::string getPos(int i);
  void setupaide();
  void ValiderScore();
  void chargementparametres(int j, int jrestants, int tour, int nbhisto, int payback[6], int estVulnerable[6], int positionDuJoueur[6], int joueurEnPosition[6], std::string historique[6], int ***Case, int ***Case2, int **casesRestantes, int ***posNavires, int *naviresRestants, int **NbPivotements, int *missilesTires, int *missilesGagnants, int tailleFlotte);
  void CreationLog();
  void Log_AjouterAction(int sc);
  void RetourMenu();
  void sauvegarde();

private:
  int nbjoueurs;
  int humains;
  Window info;
  Window plateau;
  Window aide;
  int const difficulte;

  int vitesse;
  unsigned int JoueursRestants;
  const std::string fichierlog;

  bool partiechargee;

  std::string nom[6];

  bool stop;

  int joueur;
  int cible;
  int cibleSelectionnee;
  // Gère les affinités, utilisé par l'IA.
  // Selon le mode de difficulté, renvoie et cible le dernier joueur ayant attaqué avec succès le joueur en train de jouer.
  int payback[6];
  int estVulnerable[6];
  int positionDuJoueur[6];
  int joueurEnPosition[6];
  int nbhisto;
  int tour;
  std::string historique[6];
  Color separateur[5];

  Grille *Joueur[];

  int scoreWidth;
  int syf2;
  int sxf2;
  int sy2, sx2;

  // Options du thème
  Color colScore;
  Color colAide;
  Color colBordureGrilles;
  Color colBordurePlateau;

  int sx, sy, sxf, syf;
};

int getScoreStartX(int t);
int getBordureWidth(int nbjoueurs);
int getBordureHeight(int nbjoueurs);
int getBordureStartY(int nbjoueurs);
int getAideStartY(int nbjoueurs);

void checkSpeed(int ch, Window &aide, int &vitesse);

#endif
