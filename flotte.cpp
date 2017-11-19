#include "flotte.h"
#include "window.h"
#include "config.h"
#include "cmath"
#include "iostream"
#include "algorithm"
#include <unistd.h>

using namespace std;

Flotte::Flotte(int sx, int sy) : fenetre(getDimFlotte('h'), 2 * (getDimFlotte('w')), sx, sy)
{
    init();
    refreshPort(1);
}

Flotte::~Flotte() {}

void Flotte::estAuPort(int n, bool b)
{
    if (b)
        estauport[n] = 1;
    else
    {
        estauport[n] = 0;
    }

    int x = 0;
    int xx = 0;

    for (int i = 0; i < n; i++)
    {
        x += 1 + dimNavire[i][0];
    }
    
    int fx = x + dimNavire[n][0];

    while (x < fx)
    {
        for (int y = 0; y < dimNavire[n][1]; y++)
        {
            if (navire[n][xx][y] == 1)
            {
                if (!(b))
                {
                    fenetre.print(2 * x, y, ' ');
                    fenetre.print(2 * x + 1, y, ' ');
                }
                else
                {
                    fenetre.print(2 * x, y, ' ', color[n]);
                    fenetre.print(2 * x + 1, y, ' ', color[n]);
                }
            }
        }
        x++;
        xx++;
    }

    refreshPort(0);
}

int Flotte::getNavireSuivant(int n)
{
    for (int i = n + 1; i < 5; i++)
    {
        if (estauport[i])
        {
            return i;
        }
    }
}

Color Flotte::getColor(int n)
{
    return color[n];
}

int Flotte::getNavirePrecedent(int n)
{
    for (int i = n - 1; i >= 0; i--)
    {
        if (estauport[i])
        {
            return i;
        }
    }
}

bool Flotte::getEstAuPort(int n)
{
    return estauport[n];
}

void Flotte::selectionne(int n, bool x)
{
    if (x)
        s[n] = '#';
    else
        s[n] = ' ';
}

void Flotte::echangeSelection(int n, int n2)
{
    swap(s[n], s[n2]);
}

void Flotte::supprimerduport(int n)
{
    int x = 0;

    for (int i = 0; i < n; i++)
    {
        x += 1 + dimNavire[i][0];
    }
    int fx = x + dimNavire[n][0];

    while (x < fx)
    {
        for (int y = 0; y < dimNavire[n][1]; y++)
        {
            fenetre.print(2 * x, y, ' ');
            fenetre.print(2 * x + 1, y, ' ');
        }
        x++;
    }

    refreshPort(0);
}

void Flotte::refreshPort(int delai)
{
    int x = 0;
    int y = 0;
    int x2 = 0;

    for (int n = 0; n < 5; n++)
    {
        if (estauport[n])
        {

            while (x < dimNavire[n][0])
            {
                for (y = 0; y < dimNavire[n][1]; y++)
                {
                    if (navire[n][x][y] == 1)
                    {
                        if (estauport[n])
                        {
                            fenetre.print(2 * x2, y, s[n], color[n]);
                            fenetre.print(2 * x2 + 1, y, s[n], color[n]);
                        }
                    }
                }
                x++;
                x2++;
            }
        }
        else
        {
            x2 += dimNavire[n][0];
        }

        x2++;
        x = 0;
        if (delai == 1)
            usleep(60000);
    }
}

int Flotte::getPremierNavire()
{
    for (int n = 0; n < 5; n++)
    {
        if (estauport[n])
        {
            return n;
        }
    }
}

void Flotte::initSelection()
{
    s[getPremierNavire()] = '#';
    refreshPort(0);
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

    return dimNavire[n][1];
}

int Flotte::getWidthnavire(int n)
{
    return dimNavire[n][0];

}