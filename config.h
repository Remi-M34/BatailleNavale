#ifndef __CONFIG_H
#define __CONFIG_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>

//Accès en lecture

//Retourne la longueur ou la hauteur de la fenetre de la flotte selon l'option (h ou w) 
const int getDimFlotte(char c);

const int getWidthGrille();
const int getHeightGrille();
int ***listedesnavires();
int **dimN();



#endif
