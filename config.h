#ifndef __CONFIG_H
#define __CONFIG_H

#include "window.h"
extern "C" {
#include <curses.h>
}
#include <string>

//Accès en lecture
const int getWidthFlotte();
const int getWidthGrille();
const int getHeightGrille();
int ***listedesnavires();



#endif
