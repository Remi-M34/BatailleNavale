#ifndef __FLOTTE_H
#define __FLOTTE_H

#include "../include/window.h"
#include "../include/config.h"

#include <string>

class Flotte
{

public:
  Window fenetre;

  Flotte(int sx, int sy, int delai, bool partiechargee);
  ~Flotte();

  void initDim();

  // Affiche les navires les uns après les autres si delai = 1. Sinon instantané.
  void refreshPort(int delai);

  //Lecture
  // Les dimensions du navire. Sont échangées à chaque rotation.
  int getHeightnavire(int n);
  int getWidthnavire(int n);

  // Dimensions originales du navire.
  int getHeightnavireOriginale(int n);
  int getWidthnavireOriginale(int n);

  // Renvoie le premier navire se situant au port.
  int getPremierNavire();
  int getDernierNavire();

  // Renvoie un navire aléatoire du port
  int getRandomNavire();

  // Retourne le navire suivant qui est au port
  int getNavireSuivant(int n);

  int getNavirePrecedent(int n);

  // Vérifie si le navire est actuellement séléctionné #
  bool estSelectionne(int n);

  // Vérifie si le navire est actuellement au port
  bool getEstAuPort(int n);

  Color getColor(int n);

  int **dimensionsOriginales(int n);


  //Ecriture

  //Supprime le navire du port et l'actualise
  void supprimerduport(int n);

  void initSelection();
  //Choisir si le navire n est au port. Le port est ensuite actualisé.
  void estAuPort(int n, bool x);

  // Renvoie vrai si le port est vide
  bool portVide();
  
  // Sélectionne le navire
  void selectionne(int n, bool x);

  // Selectionne et dé-selectionne chacun des deux navires.
  void echangeSelection(int n, int n2);

  // Echange la hauteur et la largeur du navire quand celui-ci est pivoté
  void swapDimensionsNavire(int n);

  void couleursNavires();
  void chargerDim(int **dim);

private:
  bool estauport[5];
  // Caractère de sélection
  char s[5];

  Color color[5];

  int ***navire;

  // Les dimensions des navires. Hauteur et largeur sont échangées à chaque rotation du navire.
  int **dimNavire;

  // Les dimensions originales du navire (hauteur, largeur)
  int **dimNavireOriginales;
};

#endif
