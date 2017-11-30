#ifndef __GRILLE_H
#define __GRILLE_H

#include "window.h"
#include "flotte.h"
#include "config.h"
#include <unistd.h>

extern "C" {
#include <curses.h>
}
#include <string>

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

  //constructeur :
  Grille(int const sx, int const sy, int sxf, int syf);
  Grille(int const sx, int const sy, int sxf, int syf, bool estIA);

  //destructeur
  ~Grille();

  void init();

  void moveNavire(int n, int sx, int sy);
  void pivoteDroite(int n, int &x, int &y);
  void checkRepositionnement(int n);
  void repositionnementVertical(int n);
  void repositionnementHorizontal(int n);
  bool checkPlacement(int n, int sx, int sy);
  bool check(int n, int sx, int sy);
  void changerEtat(int x, int y, etat e);

  //Selectionne le navire depuis le port
  void selectionNavire();
  void refreshGrille(int sx, int sy, int maxX, int maxY);
  void refreshNavireGrille(int n, int sx, int sy);

  // Placement du navire sélectionné sur la grille
  void placementNavire(int n);
  // Place tous les navires aléatoirement
  void placementAleatoire();

  void validerNavire(int n, int sx, int sy);

  // Sélectionne la case sur laquelle tirer un missile
  int destinationMissile();
  int destinationMissileAleatoire();
  void moveRight(int &x, int &y);
  void moveLeft(int &x, int &y);
  void moveUp(int &x, int &y);
  void moveDown(int &x, int &y);
  void caseSuivante(int &x, int &y);

  // Trouve et sélectionne la case la plus proche du milieu de la grille qui n'a pas déjà été victime d'un tir de missile
  void findMilieu(int &x, int &y);
  void zoneFocus(int &x, int &y);
  bool aDesVoisins(int x, int y);
  bool caseNonDecouverte(int x, int y);
  int findFocus();
  void caseAleatoire(int &x, int &y);

  bool estCoule(int n);

  void coulerNavire(int n);
  void initCouleurs();
  void test(std::string s);

private:
  etat **Case;
  int **Case2;
  bool IA = false;
  int ***navire = listedesnavires();

  // Position du navire sur la grille. [n][0] = x, [n][1] = y; afin de changer l'état
  // de toutes ses cases quand il est coulé
  int posNavire[5][2];
  int focusnavire = -1;
  int focusx = -1;
  int focusy = -1;

  // +1 pour chaque case dont dispose le navire. -1 à chaque missile tiré sur ce navire. A 0, le navire est coulé.
  // Est initialisé lors de la création de la grille
  int caseRestantes[5] = {0};

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
};

std::string myitoa(int i);

#endif
