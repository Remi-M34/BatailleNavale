#ifndef __PLATEAU_H
#define __PLATEAU_H

#include "window.h"
#include "grille.h"
extern "C" {
#include <curses.h>
}
#include <string>





class Plateau{

public:

    Grille joueur;
    Grille ennemi;

    //constructeur :
    Plateau(int h, int w, int sx, int sy, int nbj);

    //destructeur
    ~Plateau();





private:


     int height ;
     int width ;
    int startX, startY;
    int nbjoueurs;
    


};





#endif
