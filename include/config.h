#ifndef __CONFIG_H
#define __CONFIG_H

#include "../include/window.h"
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

// Cherche une couleur dans une chaine de caractère string et la renvoie en tant que Color
Color convertColor(std::string ligne);

void changeTheme(std::string th);
void changePreset(int n);

void editWidthGrille(int w);
void editHeightGrille(int h);
std::string to_string(int i);

void erreurEcran(int w, int h);

//BUG --- Ne pas utiliser
int **dimN();




#endif
