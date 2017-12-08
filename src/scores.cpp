#include "../include/headers.h"

using namespace std;


int Scores::getScores()
{
    return (missilesGagnants/missilesTires)/(height*width);
}