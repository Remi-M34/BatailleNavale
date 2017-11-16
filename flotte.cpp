#include "flotte.h"
#include "window.h"
#include "config.h"
#include "cmath"
#include "iostream"

using namespace std;

Flotte::Flotte(int sx, int sy) : startx(sx), starty(sy), fenetre(5, 4+2*getWidthFlotte(), sx, sy) {}

Flotte::~Flotte() {}



