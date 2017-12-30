#ifndef __MENU_H
#define __MENU_H

#include "../include/window.h"
#include "../include/flotte.h"

#include <string>

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
  void topscores(Window& top);
void refreshMenu();
    void aideMenu();
    void preset(int s);
    void chargement();
    // Vérifie si la taille de l'écran est assez grande pour lancer le jeu.
    void checkTailleEcran();
    // Active et place le curseur à la position séléctionnée de la fenêtre. Renvoie un string.
    std::string saisie(int x, int y, int longueur, Window &p);
    Menu();
    ~Menu();

  private:
    Window plateau;

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
    Color colBordureFlotte;
    bool bordureMenuFine = false;
    char carBordureMenu = ' ';



    std::string theme;
};

void aideOptionsJeu(Window &aide,int s);


#endif
