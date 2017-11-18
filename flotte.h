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

  void init();
  void port();
  void Select();

  //Lecture
  int getHeightnavire(int n);
  int getWidthnavire(int n);

private:
  bool estauport[5] = {1, 1, 1, 1, 1};
  char s[5] = {'#', ' ', ' ', ' ', ' '};

  Color color[5] = {WGREEN,
                    WMAGENTA,
                    WYELLOW,
                    WBLUE,
                    WRED
                    };

  int sel = 5;

  int startx;
  int starty;
  char bord;
  int ***navire = listedesnavires();
  int *heightnavire;
  int *widthnavire;
};

#endif
