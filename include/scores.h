#ifndef __SCORES_H
#define __SCORES_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>
#include <iostream>
#include "config.h"

#include <cmath>

class Scores
{

public:

  Scores();
  ~Scores();


  int getScores();

private:

int missilesTires = 0;
int missilesGagnants = 0;

int nbjoueurs = 0;
int height = getHeightGrille();
int width = getWidthGrille();

int score = 0;


};

#endif
