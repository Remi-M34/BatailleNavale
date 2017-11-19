#ifndef __NAVIRE_H
#define __NAVIRE_H

#include "window.h"
#include "config.h"
extern "C" {
#include <curses.h>
}
#include <string>






class Navire{

public:


    //constructeur :
    Navire();

    //destructeur
    ~Navire();



private:


    int*** navire = listedesnavires();
    int*** navirePivote = listedesnavires();
    int decalageGauche[5] = {0,0,0,0};
    int decalageDroite[5] = {0,0,0,0};
    int decalageHaut[5] = {0,0,0,0};
    int decalageBas[5] = {0,0,0,0};


};





#endif
