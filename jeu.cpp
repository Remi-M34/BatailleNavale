#include "jeu.h"
#include "window.h"
#include "config.h"
#include "grille.h"
#include <unistd.h>
#include "string"
using namespace std;

#define H getHeightGrille()
#define W getWidthGrille()


Jeu::Jeu(int sx,int sy, int sxf, int syf, int sx2, int sy2, int sxf2, int syf2) : joueur1(sx,sy,sxf,syf), joueur2(sx2,sy2,sxf2,syf2)
{


}


Jeu::~Jeu() {}