#include "grille.h"
#include "window.h"


Grille::Grille(int h, int w, int sx, int sy) : height(h), width(w), startx(sx), starty(sy), fenetre(h,w,sx,sy){}

Grille::~Grille(){}