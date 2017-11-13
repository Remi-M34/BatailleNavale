#ifndef __CASE_H
#define __CASE_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>


enum e {
    VIDE,
    NAVIRE,
    TOUCHE,
    COULE
};



class Case{

public:

    //constructeur :
    Case(int h, int w);

    //destructeur
    ~Case();

    int **c;




private:


    const int height ;
    const int width ;
    


};





#endif
