#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "window.h"
#include "config.h"
extern "C" {
#include <curses.h>
}
#include <string>



std::string insPlacement();
std::string aide();
std::string aPropos();
std::string version();
std::string infoFlotte(int s);
std::string infoFlotteD(int s);
std::string infoFlotteT();




#endif