#ifndef __CONFIG_H
#define __CONFIG_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>

//Accès en lecture

//Retourne la longueur ou la hauteur de la fenetre de la flotte selon l'option (h ou w) 
//BUG --- Ne pas utiliser
const int getDimFlotte(char c);

const int getWidthGrille();
const int getHeightGrille();
int ***listedesnavires();

void modifierNavires(int n, int width, int height, int** navire);
void ecrireNavire(int n);

// Lis une ligne et renvoie la couleur
Color convertColor(std::string ligne);

void changeTheme(std::string th);

void editWidthGrille(int w);
void editHeightGrille(int h);

//BUG --- Ne pas utiliser
int **dimN();




#endif
