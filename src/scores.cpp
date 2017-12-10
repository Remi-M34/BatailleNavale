#include "../include/scores.h"

#include <cmath>
#include <iostream>

using namespace std;


Scores::Scores(int difficulte) : difficulte(difficulte)
{}

Scores::~Scores(){}

int Scores::getScore()
{
    double s = ((double)missilesGagnants/missilesTires)*(12500)/((double)tailleFlotte/(height*width));
    // cout << s << endl;
    if (missilesTires == 0)
    {
        return 0;
    }
    else return ceil(s);
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