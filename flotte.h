#ifndef __FLOTTE_H
#define __FLOTTE_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>
#include <iostream>
#include <cmath>





class Flotte{

public:


    Window fenetre; 

    int places = 0;
    //constructeur :
    Flotte(int h, int w, int sx, int sy);

    //destructeur
    ~Flotte();


    void present();
    void first();
    void selection(int s, int d);
    int taille(int i);
    bool selected(int i);



private:


    int n[5] = {1,1,1,1,1};

    char s[5] = { '#',' ',' ',' ',' ' };


    int sel = 5;

    int height;
    int width;
    int startx;
    int starty;
    char bord;

    int posX,PosX2,PosY;

    



    


};





#endif
