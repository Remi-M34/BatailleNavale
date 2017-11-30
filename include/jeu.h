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

  Jeu(int sx, int sy, int sxf, int syf, int sx2, int sy2, int sxf2, int syf2, int scoreHeight, int scoreWidth, int sxScore, int syScore, int xm, int ym);

  //destructeur
  ~Jeu();

void  placement();
void  tir(Grille joueur);
void initCouleurs();


private:
      Window score;
      Grille joueur1;
      Grille joueur2;

      Color colScore;

     bool Joueur1Humain = 0;
     bool Joueur2Humain = 0;

};



#endif
