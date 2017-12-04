#include "config.h"
#include "flotte.h"
#include "navire.h"
#include "grille.h"
#include "jeu.h"
#include "menu.h"
#include "message.h"
#include "window.h"

#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <cstdio>
#include <cassert>

extern "C" {
#include <curses.h>
}