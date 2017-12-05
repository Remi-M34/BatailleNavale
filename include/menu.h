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
    void selectJoueurs(Window plateau);

    void aideMenu();
    void preset(int s);
    // Vérifie si la taille de l'écran est assez grande pour lancer le jeu.
    void checkTailleEcran();
    // Active et place le curseur à la position séléctionnée de la fenêtre. Renvoie un string.
    std::string saisie(int x, int y, int longueur, Window &p);
    Menu();
    ~Menu();

  private:
    std::string nom[6];

    int nbjoueurs = 0;
    int nbjoueurshumain = 0;
    int vitesse = 6;
   // Gère le niveau de difficulté, entre 1 et 3. Moyen par défaut.
    int difficulte = 2;


    // Options du thème
    Color colSelectionnee;
    Color colNonSelectionnee;
    Color colBordureMenu;
    bool bordureMenuDroite = false;
    char carBordureMenu = ' ';



    std::string theme;
};

void aideOptionsJeu(Window &aide,int s);


#endif
