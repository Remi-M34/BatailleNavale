#include "../include/scores.h"

#include <cmath>
#include <iostream>

using namespace std;

Scores::Scores(int difficulte) : difficulte(difficulte)
{
  missilesTires = 0;
  missilesGagnants = 0;
  tailleFlotte = 0;
  height = getHeightGrille();
  width = getWidthGrille();
}

Scores::~Scores() {}

int Scores::getScore()
{
  double s = ((double)missilesGagnants / missilesTires) * (12500) / ((double)tailleFlotte / (height * width));
  if (missilesTires == 0)
  {
    return 0;
  }
  else
    return ceil(s);
}

void Scores::missileEnvoye()
{
  missilesTires++;
}

void Scores::navireTouche()
{
  missilesTires++;
  missilesGagnants++;
}

void Scores::augmenterTailleFlotte()
{
  tailleFlotte++;
}

int Scores::getMissilesTires()
{

  return missilesTires;
}
int Scores::getMissilesGagnants()
{

  return missilesGagnants;
}

int Scores::getTailleFlotte()
{

  return tailleFlotte;
}

void Scores::ChargerDonnees(int MissilesTires, int MissilesGagnants, int tailleFlotte)
{
  this->missilesTires = MissilesTires;
  this->missilesGagnants = MissilesGagnants;
  this->tailleFlotte = tailleFlotte;
}