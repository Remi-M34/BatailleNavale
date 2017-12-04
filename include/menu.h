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
    void mainMenu(int d);
    void initCouleurs();
    void changerDim();
    void menuNavire();
    void themes();
    void optionsJeu();
    void erreurEcran();
    void selectJoueurs(Window plateau);

    void aideMenu();
    void preset(int s);
    void checkTailleEcran();
    std::string saisie(int x, int y, int longueur, Window &p);
    Menu();
    ~Menu();

  private:
    std::string nom[6];

    int nbjoueurs = 0;
    int nbjoueurshumain = 0;
    int vitesse = 6;
    int difficulte = 1;
    Color colSelectionnee;
    Color colNonSelectionnee;
    Color colBordureMenu;
    bool bordureMenuDroite = false;
    char carBordureMenu = ' ';

    std::string theme;
};

void aideOptionsJeu(Window &aide,int s);


#endif
