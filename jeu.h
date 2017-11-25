#ifndef __JEU_H
#define __JEU_H

#include "window.h"
#include "config.h"
#include "grille.h"
extern "C" {
#include <curses.h>
}
#include <string>



class Jeu{

public:

  Jeu(int sx, int sy, int sxf, int syf, int sx2, int sy2, int sxf2, int syf2);

  //destructeur
  ~Jeu();

      Grille joueur1;
      Grille joueur2;


private:


};



#endif
