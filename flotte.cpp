#include "flotte.h"
#include "window.h"
#include "config.h"
#include "cmath"
#include "iostream"
#include <unistd.h>

using namespace std;

Flotte::Flotte(int sx, int sy) : startx(sx), starty(sy), fenetre(getDimFlotte('h'), 2 * (getDimFlotte('w')), sx, sy)
{
    init();
    port();
}

Flotte::~Flotte() {}




void Flotte::Select()
{
    int ch;
    while ((ch = getch()) != '\n')
    {
        port();
        switch (ch)
        {
            case 'KEY_RIGHT':

            break;

            case 'KEY_LEFT':

            break;

            case '\n':

            break;
        }
    }
}




void Flotte::port()
{
    int x = 0;
    int y = 0;
    int x2 = 0;

    for (int n = 0; n < 5; n++)
    {

        if (estauport[n])
        {
            while (x < widthnavire[n])
            {
                for (y = 0; y < heightnavire[n]; y++)
                {
                    if (navire[n][x][y] == 1)
                    {
                        fenetre.print(2 * x2, y, s[n], color[n]);
                        fenetre.print(2 * x2 + 1, y, s[n], color[n]);
                    }
                }
                x++;
                x2++;
            }
        }
        else
        {
            x2 += widthnavire[n];
        }
        x2++;
        x = 0;
        usleep(60000);
    }
}

//Obtention des longueurs et largeurs de chaque navire
void Flotte::init()
{
    heightnavire = new int[5];
    widthnavire = new int[5];
    for (int n = 0; n < 5; n++)
    {
        heightnavire[n] = getHeightnavire(n);
        widthnavire[n] = getWidthnavire(n);
    }
}

int Flotte::getHeightnavire(int n)
{
    int h = 0;
    while (navire[n][0][h] != -1)
        h++;

    return h;
}

int Flotte::getWidthnavire(int n)
{
    int w = 0;
    while (navire[n][w][0] != -1)
        w++;

    return w;
}