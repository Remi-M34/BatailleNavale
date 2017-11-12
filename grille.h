#ifndef __GRILLE_H
#define __GRILLE_H

#include "window.h"
#include "flotte.h"

extern "C" {
#include <curses.h>
}
#include <string>






class Grille{

public:

    Window fenetre; 
    Flotte flotte;
    //constructeur :
    Grille(int h, int w, int sx, int sy);


    //destructeur
    ~Grille();

    void placement(int t);



private:

    int height;
    int width;
    int startx;
    int starty;
    char bord;

    int posX,PosX2,PosY;



    


};





#endif
