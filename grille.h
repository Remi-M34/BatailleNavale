#ifndef __GRILLE_H
#define __GRILLE_H

#include "window.h"
#include "flotte.h"
#include "config.h"

#include <unistd.h>

extern "C" {
#include <curses.h>
}
#include <string>

enum etat
{
    VIDE = 0,
    VIDETOUCHE = 1,
    NAVIRE = 2,
    TOUCHE = 3,
    COULE = 4
};

class Grille
{

  public:
    Window fenetre;
    Flotte flotte;

    //constructeur :
    Grille(int const sx, int const sy, int sxf, int syf);

    //destructeur
    ~Grille();

    void init();

  private:
    etat **Case;
};

#endif
