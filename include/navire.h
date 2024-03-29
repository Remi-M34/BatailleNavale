#ifndef __NAVIRE_H
#define __NAVIRE_H

#include "../include/config.h"


// Sert à l'édition des navires depuis le menu


void nouveauNavire(int n);
void moveNavire(Window creation, int x, int y , char d);
int getDim(int** navire);

// Vérifie que le navire est le plus en haut à gauche possible (à la fin de l'édition)
int** checkRepositionnement(int** navire);
int** repositionnementHorizontal(int** navire);
int** repositionnementVertical(int** navire);



#endif
