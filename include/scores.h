#ifndef __SCORES_H
#define __SCORES_H

#include "../include/config.h"


class Scores
{

public:

  Scores(int difficulte);
  ~Scores();

  // Renvoie le score arrondi
  int getScore();
  void missileEnvoye();
  void navireTouche();

  // La taille de la flotte correspond au nombre total de cases que possèdent chaque navire (entre 5 et 125)
  void augmenterTailleFlotte();

  int getMissilesTires();
  int getMissilesGagnants();
  int getTailleFlotte();

  void ChargerDonnees(int MissilesTires, int MissilesGagnants, int tailleFlotte);

private:

int missilesTires;
int missilesGagnants;
int tailleFlotte;

int nbjoueurs;
int height;
int width;

int difficulte;


};

#endif
