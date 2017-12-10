#ifndef __SCORES_H
#define __SCORES_H

#include "../include/config.h"


class Scores
{

public:

  Scores(int difficulte);
  ~Scores();


  int getScore();

private:

int missilesTires = 0;
int missilesGagnants = 0;

int nbjoueurs;
int height = getHeightGrille();
int width = getWidthGrille();

int difficulte;


};

#endif
