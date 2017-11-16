#include "grille.h"
#include "plateau.h"
#include "config.h"

Plateau::Plateau(int h, int w, int sx, int sy, int nbj) : height(h), width(w), startX(sx), startY(sy), joueur(h,w,sx,sy), ennemi(h,w,sx+25,sy), nbjoueurs(nbj){}

Plateau::~Plateau(){}

