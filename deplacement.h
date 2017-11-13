#ifndef __FLOTTE_H
#define __FLOTTE_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>






class Flotte{

public:

    Window fenetre; 

    //constructeur :
    Flotte(int h, int w, int sx, int sy);

    //destructeur
    ~Flotte();



private:

    int height;
    int width;
    int startx;
    int starty;
    char bord;

    int posX,PosX2,PosY;



    


};





#endif
