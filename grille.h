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
  void validerNavire(int n, int sx, int sy);

  // Sélectionne la case sur laquelle tirer un missile
  int destinationMissile();
  void moveRight(int &x, int &y);
  void moveLeft(int &x, int &y);
  void moveUp(int &x, int &y);
  void moveDown(int &x, int &y);

  // Trouve et sélectionne la case la plus proche du milieu de la grille qui n'a pas déjà été victime d'un tir de missile
  void findMilieu(int &x, int &y);

  bool estCoule(int n);

  void coulerNavire(int n);
  void initCouleurs();
  void test();

private:
  etat **Case;
  int **Case2;
  int ***navire = listedesnavires();

  // Position du navire sur la grille. [n][0] = x, [n][1] = y; afin de changer l'état
  // de toutes ses cases quand il est coulé
  int posNavire[5][2];

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
};

#endif
