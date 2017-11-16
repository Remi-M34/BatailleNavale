#ifndef __FLOTTE_H
#define __FLOTTE_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>
#include <iostream>
#include "config.h"

#include <cmath>

class Flotte
{

  public:
    Window fenetre;

    int places = 0;
    //constructeur :
    Flotte(int sx, int sy);

    //destructeur
    ~Flotte();

  


  private:
    int n[5] = {1, 1, 1, 1, 1};
    char s[5] = {'#', ' ', ' ', ' ', ' '};

    int sel = 5;


    int startx;
    int starty;
    char bord;

};

#endif
