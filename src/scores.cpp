#include "../include/scores.h"

using namespace std;


Scores::Scores(int difficulte) : difficulte(difficulte)
{}

Scores::~Scores(){}

int Scores::getScore()
{
    return (missilesGagnants/missilesTires)/(height*width);
}