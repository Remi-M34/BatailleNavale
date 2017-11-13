#ifndef __GRILLE_H
#define __GRILLE_H

#include "window.h"
#include "flotte.h"
#include <unistd.h>


extern "C" {
#include <curses.h>
}
#include <string>


enum etat {
    VIDE = 0,
    VIDETOUCHE = 1,
    NAVIRE = 2,
    TOUCHE = 3,
    COULE = 4
};


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

    etat **Case;

    int posX,PosX2,PosY;



    


};





#endif
