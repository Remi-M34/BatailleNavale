#ifndef __CONFIG_H
#define __CONFIG_H

#include "../include/window.h"
#include <string>

// Le fichier config possède les fonctions du menu, nécessitant la lecture et l'écriture dans des fichiers

//Accès en lecture

//Retourne la longueur ou la hauteur de la fenetre de la flotte selon l'option (h ou w) 
 int getDimFlotte(char c);

 int getWidthGrille();
 int getHeightGrille();
int ***listedesnavires();


// Modifie les navires depuis le menu
void modifierNavires(int n, int width, int height, int** navire);
void ecrireNavire(int n);

// Cherche une couleur dans une chaine de caractère string et la renvoie en tant que Color (enum)
Color convertColor(std::string ligne);

void changeTheme(std::string th);
void changePreset(int n);

void editWidthGrille(int w);
void editHeightGrille(int h);
std::string to_string(int i);

void erreurEcran();




#endif
