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
  COULE = 4,
  TOUCHECOULE = 5
};

class Grille
{

public:
  Window fenetre;
  Flotte flotte;
    Scores Score;


  //Constructeur :
  Grille(int const sx, int const sy, int sxf, int syf, int &vitesse, int const difficulte, Window &aide);
  Grille(int const sx, int const sy, int sxf, int syf, int &vitesse, int const difficulte, Window &aide, bool estIA);

  //Destructeur
  ~Grille();

  void init();

  void moveNavire(int n, int sx, int sy);
  void pivoteDroite(int n, int &x, int &y);
  void checkRepositionnement(int n);
  void repositionnementVertical(int n);
  void repositionnementHorizontal(int n);
  bool checkPlacement(int n, int sx, int sy);
  bool check(int n, int sx, int sy);

  //Changer l'état d'une case (vide,touché,occupée,coulé..)
  void changerEtat(int x, int y, etat e);

  // Modifie la vitesse de l'IA pendant qu'elle joue
  void checkSpeedG();

  //Selectionne le navire depuis le port
  void selectionNavire();

  //Rafraichis la grille depuis un point donné jusqu'à un autre (afin de réduire la latence).
  //(0,0,W,H) rafrachira la grille entière.
  void refreshGrille(int sx, int sy, int maxX, int maxY);

  //Rafraichis une partie de la grille sur laquelle se trouve un navire afin de ne modifier que celui-ci, selon son état.
  void refreshNavireGrille(int n, int sx, int sy);

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
  void moveRight(int &x, int &y);
  void moveLeft(int &x, int &y);
  void moveUp(int &x, int &y);
  void moveDown(int &x, int &y);
  // Se positionne sur la case suivante. A utiliser à la fin du jeu pour éviter de se retrouver bloquer.
  void caseSuivante(int &x, int &y);
  void casePrecedente(int &x, int &y);
  // Cache toutes les cases de la grille.
  void cacherCases();
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
  int findFocus();
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
  // Initialise les couleurs du thème.
  void initCouleurs();

  // Ecrit un string dans le fichier test.txt. Sert au débuggage.

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

private:
// L'etat de chaque case.
  etat **Case;
  // Renvoie le numéro du navire placé sur la case choisie. Renvoie -1 si aucun navire ne se trouve sur la case.
  int **Case2;
  bool IA = false;
  // La liste des navires, ou plutôt des tableaux dans lesquels ils se trouvent.
  int ***navire = listedesnavires();
  // La vitesse du jeu (IA) entre 0 et 9. Est passé en référence depuis la classe Jeu.
  // Ainsi, une vitesse changée sur une grille le sera sur toutes les autres.
  int &vitesse;
  int const difficulte;
  bool joue = false;
  // Permet d'afficher la nouvelle vitesse dans la fenêtre appropriée.
  Window *aide;


  // Position du navire sur la grille. [n][0] = x, [n][1] = y; afin de changer l'état
  // de toutes ses cases quand celui-ci est coulé
  int posNavire[5][2];
  // Si le focus == -1, une case aléatoire sera séléctionnée.
  int focusnavire = -1;
  int focusx = -1;
  int focusy = -1;

  // Un joueur ayant tiré sur un navire ne sera pas renvoyé au milieu de la grille si la valeur != -1.
  int dernierePositionGagnante[2] = {-1};

  // +1 pour chaque case dont dispose le navire. -1 à chaque missile tiré sur ce navire. A 0, le navire est coulé.
  // Est initialisé lors de la création de la grille
  int caseRestantes[5] = {0};

  // Selon les options du thème
  Color colTouche;
  Color colCoule;
  Color colManque;
  Color colVide;
  Color colCaseSelectionnee;
  Color bordure;
  Color colMauvaiseCouleur;
  Color colNavires;
  int delaiTirIA = 0;
  int delaiPoseIA = 0;
  char carBordureGrille;
  char carGrilleMauvaiseSelection = '!';
};

std::string myitoa(int i);
void test(std::string s);

#endif
