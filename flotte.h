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
  Flotte(int sx, int sy);

  //destructeur
  ~Flotte();

  void init();

  // Affiche les navires les uns après les autres si delai = 1
  void refreshPort(int delai);

  //Supprimer le navire du port et l'actualise
  void supprimerduport(int n);

  //Lecture
  int getHeightnavire(int n);
  int getWidthnavire(int n);
  int getPremierNavire();

  // Retourne le navire suivant qui est au port
  int getNavireSuivant(int n);
  int getNavirePrecedent(int n);
  bool estSelectionne(int n);
  bool getEstAuPort(int n);

  Color getColor(int n);

  //Ecriture

  void initSelection();
  //Choisir si le navire n est au port. Le port est ensuite actualisé.
  void estAuPort(int n, bool x);
  void selectionne(int n, bool x);
  void echangeSelection(int n, int n2);




private:
  bool estauport[5] = {1, 1, 1, 1, 1};
  bool estplace[5] = {0, 0, 0, 0, 0};
  char s[5] = {' ', ' ', ' ', ' ', ' '};

  Color color[5] = {WGREEN,
                    WMAGENTA,
                    WYELLOW,
                    WBLUE,
                    WRED
                    };


  int *heightnavire;
  int *widthnavire;

      int ***navire = listedesnavires();


  int **dimNavire = dimN();

};

#endif
