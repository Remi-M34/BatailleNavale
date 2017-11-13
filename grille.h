#ifndef __GRILLE_H
#define __GRILLE_H

#include "window.h"
#include "flotte.h"
#include "case.h"
#include <unistd.h>


extern "C" {
#include <curses.h>
}
#include <string>





class Grille{

public:

    Window fenetre; 
    Flotte flotte;
    //constructeur :
    Grille(int const h, int const w, int sx, int sy);


    //destructeur
    ~Grille();

    void placement(int t);
    void init(int const h, int const w);


private:

    int const height;
    int const width;
    int startx;
    int starty;
    char bord;

    int Case[20][10];


    int posX,PosX2,PosY;



    


};





#endif
