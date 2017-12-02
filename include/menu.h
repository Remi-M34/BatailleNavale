#ifndef __MENU_H
#define __MENU_H

#include "window.h"
#include "config.h"
extern "C" {
#include <curses.h>
}
#include <string>
#include "window.h"
#include "flotte.h"

class Menu
{
    public:
    void options();
    void mainMenu();
    void initCouleurs();
    void changerDim();
    void menuNavire();
    void themes();

    void aideMenu();
    void preset( int s);

    Menu();
    ~Menu();


    private:

        Color colSelectionnee;
    Color colNonSelectionnee;
    Color colBordureMenu;
    bool bordureMenuDroite = false;
    char carBordureMenu = ' ';

    std::string theme;
};






#endif