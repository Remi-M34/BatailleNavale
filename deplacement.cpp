#include "flotte.h"
#include "window.h"


Flotte::Flotte(int h, int w, int sx, int sy) : height(h), width(w), startx(sx), starty(sy), fenetre(h,w,sx,sy){}

Flotte::~Flotte(){}