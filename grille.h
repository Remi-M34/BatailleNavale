#ifndef __GRILLE_H
#define __GRILLE_H

#include "window.h"
#include "flotte.h"
#include "config.h"
#include "navire.h"

#include <unistd.h>

extern "C" {
#include <curses.h>
}
#include <string>

enum etat
{
  VIDE = 0,
  VIDETOUCHE = 1,
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

  void moveNavire(int n, int& sx, int& sy);
  void pivoteDroite(int n);
  void checkRepositionnement(int n);
  void repositionnementVertical(int n);
    void repositionnementHorizontal(int n);
  bool check(int n, int sx, int sy);


//Selectionne le navire depuis le port
  void selectionNavire();
  void refreshGrille();

  // Placement du navire sélectionné sur la grille
  void placementNavire(int n);
  void initDecalage();

private:
  etat **Case;
  int ***navire = listedesnavires();










};

#endif
