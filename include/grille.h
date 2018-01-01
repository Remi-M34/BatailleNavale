#ifndef __GRILLE_H
#define __GRILLE_H

#include "../include/config.h"
#include "../include/window.h"
#include "../include/flotte.h"
#include "../include/scores.h"

enum etat
{
  VIDE = 0,
  TOMBEALEAU = 1,
  NAVIRE = 2,
  TOUCHE = 3,
  COULE = 4
};

class Grille
{

public:
  //Constructeur :
  Grille(int const sx, int const sy, int sxf, int syf, int &vitesse, int const difficulte, Window &aide, bool estIA, std::string nom, bool partiechargee);

  //Destructeur
  ~Grille();

  Window fenetre;
  Flotte flotte;
  Scores Score;


  void init();

  // Initialise les couleurs du thème.
  void initCouleurs();

  //Rafraichis la grille depuis un point donné jusqu'à un autre (afin de réduire la latence).
  //(0,0,W,H) rafraichira la grille entière.
  void refreshGrille(int sx, int sy, int maxX, int maxY);

  //Rafraichit une partie de la grille sur laquelle se trouve un navire afin de ne modifier que celui-ci, selon son état.
  void refreshNavireGrille(int n, int sx, int sy);

  //Changer l'état d'une case (vide,touché,occupée,coulé..)
  void changerEtat(int x, int y, etat e);

  // Modifie la vitesse de l'IA pendant qu'elle joue
  void checkSpeedG();

  //NAVIRES
  void moveNavire(int n, int sx, int sy);
  // Pour faire pivoter vers la gauche, on appelera trois fois la fonction pivoteDroite.
  void pivoteDroite(int n, int &x, int &y);
  void checkRepositionnement(int n);
  void repositionnementVertical(int n);
  void repositionnementHorizontal(int n);
  bool checkPlacement(int n, int sx, int sy);

  //Selectionne le navire depuis le port
  void selectionNavire();

  // Placement du navire sélectionné sur la grille
  void placementNavire(int n);

  // Place tous les navires aléatoirement
  void placementAleatoire();

  // Valide la position du navire sur la grille.
  void validerNavire(int n, int sx, int sy);

  // Sélectionne la case sur laquelle tirer un missile
  int destinationMissile();
  // Sélectionne une case aléatoire sur laquelle tirer un missile et se déplace automatiquement.
  int destinationMissileAleatoire();

  // Deplacement:
  void moveRight(int &x, int &y);
  void moveLeft(int &x, int &y);
  void moveUp(int &x, int &y);
  void moveDown(int &x, int &y);
  // Se positionne sur la case suivante. A utiliser à la fin du jeu pour éviter de se retrouver bloquer.
  void caseSuivante(int &x, int &y);
  void casePrecedente(int &x, int &y);

  // Usleep pour une durée déterminée par la vitesse du jeu de l'IA.
  void wait();

  // Trouve et sélectionne la case la plus proche du milieu de la grille qui n'a pas déjà été victime d'un tir de missile
  void findMilieu(int &x, int &y);

  // Cherche une case à attaquer autour du dernier navire touché.
  // Le rayon dépendra de la difficulté.
  void zoneFocus(int &x, int &y);

  // Cherche si la case passée en paramètre possède des cases non découvertes dans un rayon de 1.
  bool aDesVoisins(int x, int y);

  // Renvoie vrai si la case n'a pas encore été touchée par un tir de missile.
  bool caseNonDecouverte(int x, int y);

  // Cherche l'existence d'un navire endommagé
  int  findFocus();

  // Renvoie une case complètement aléatoire.
  void caseAleatoire(int &x, int &y);
  void deplacementIA(int dx, int dy, int x, int y);

  // Vérifie si le chemin vertical/horizontal vers la case cible est libre.
  // Dans le cas contraire, se déplace en diagonal ou fait appel aux fonctions caseSuivante, casePrecedente.
  bool checkCheminVertical(int dx, int dy, int y);
  bool checkCheminHorizontal(int dx, int dy, int x);

  // Vérifie si le navire a été coulé.
  bool estCoule(int n);
  // Dans ce cas, change l'état de celui-ci et modifie par conséquence son apparence sur la grille.
  void coulerNavire(int n);

  //CIBLAGE
  // Le joueur a qui appartient la grille devient la cible de l'attaquant. Modifie l'apparence de la bordure.
  void setEstCible(bool c);
  void nonCible();
  bool estDejaCible();

  // Vérifie si c'est au tour du joueur a qui appartient la grille.
  bool getJoue();

  // Le joueur a qui appartient la grille joue ou non.
  void setJoue(bool c);
  bool estIA();
  void setCiblageValide(bool c);

  // Le joueur a tenté de se cibler lui-même.
  void mauvaiseSelection();
  bool toucheBord(int x, int d);

  int getScore();
  int *getDernierePositionGagnante();
  int *getDernierTir();
  bool estMort();
  // Affiche une croix immense sur la grille du joueur quand celui-ci meurt
  void croixmort();

  // Sauvegarde / chargement
  int getNaviresRestants();
  int getNbPivotements(int n);
  void SauvegarderGrille(std::fstream &save);
  void getCasesRestantes(std::fstream &save);
  void getPosNavires(std::fstream &save);
  void ChargementDonnees(int **Case, int **Case2, int *casesRestantes, int **posNavires, int naviresRestants, int *NbPivotements, int MissilesTires, int MissilesGagnants, int tailleFlotte);

private:
  bool IA;
  int &vitesse;
  Window *aide;
  int const difficulte;
  std::string nom;

  // L'etat de chaque case.
  etat **Case;
  int **Case2;
  int ***navire;
  bool joue;
  unsigned int naviresRestants;

  // Position du navire sur la grille. [n][0] = x, [n][1] = y; afin de changer l'état
  // de toutes ses cases quand celui-ci est coulé
  int posNavire[5][2];

  int focusnavire;
  int focusx;
  int focusy;

  int dernierePositionGagnante[2];
  int dernierTir[2];

  int caseRestantes[5];
  int NombrePivotements[5];
  bool retourmenu;
  bool retoursauvegarde;

  // Selon les options du thème
  Color colTouche;
  Color colCoule;
  Color colManque;
  Color colVide;
  Color colCaseSelectionnee;
  Color bordure;
  Color colMauvaiseCouleur;
  Color colNavires;
  int delaiTirIA;
  int delaiPoseIA;
  char carBordureGrille;
  char carGrilleMauvaiseSelection;
};

std::string myitoa(int i);

#endif
