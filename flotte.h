#ifndef __FLOTTE_H
#define __FLOTTE_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>
#include <iostream>
#include "config.h"

#include <cmath>

class Flotte
{

public:
  Window fenetre;

  int places = 0;
  //constructeur :
  Flotte(int sx, int sy, int delai);

  //destructeur
  ~Flotte();

  void initDim();

  // Affiche les navires les uns après les autres si delai = 1. Sinon instantané.
  void refreshPort(int delai);

  //Supprime le navire du port et l'actualise
  void supprimerduport(int n);

  //Lecture
  int getHeightnavire(int n);
  int getWidthnavire(int n);
  int getHeightnavireOriginale(int n);
  int getWidthnavireOriginale(int n);
  int getPremierNavire();
  int getDernierNavire();
  int **dimensionsOriginales(int n);

  // Retourne le navire suivant qui est au port
  int getNavireSuivant(int n);
  int getNavirePrecedent(int n);

  // Vérifie si le navire est actuellement séléctionné #
  bool estSelectionne(int n);
  // Vérifie si le navire est actuellement au port
  bool getEstAuPort(int n);

  Color getColor(int n);

  //Ecriture

  void initSelection();
  //Choisir si le navire n est au port. Le port est ensuite actualisé.
  void estAuPort(int n, bool x);
  void selectionne(int n, bool x);

  // Selectionne et dé-selectionne chacun des deux navires.
  void echangeSelection(int n, int n2);

  // Echange la hauteur et la largeur du navire quand celui-ci est pivoté
  void swapDimensionsNavire(int n);

  void couleursNavires();


private:
  bool estauport[5] = {1, 1, 1, 1, 1};
  bool estplace[5] = {0, 0, 0, 0, 0};
  // Caractère de sélection
  char s[5] = {' ', ' ', ' ', ' ', ' '};

  Color color[5] = {WBLACK};



  int ***navire = listedesnavires();

  // Les dimensions des navires. Hauteur et largeur sont échangées à chaque rotation du navire.
  int **dimNavire;

  // Les dimensions originales du navire (hauteur, largeur)
  int **dimNavireOriginales;

};

#endif
