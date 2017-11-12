#ifndef __GRILLE_H
#define __GRILLE_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>






class Grille{

public:

    Window fenetre; 

    //constructeur :
    Grille(int h, int w, int sx, int sy);

    //destructeur
    ~Grille();



private:

    int height;
    int width;
    int startx;
    int starty;
    char bord;

    int posX,Posx2,Posy;



    


};





#endif
