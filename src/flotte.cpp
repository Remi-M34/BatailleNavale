#include "../include/flotte.h"
#include "../include/window.h"

#include <iostream>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

Flotte::Flotte(int sx, int sy, int d, bool partiechargee) : fenetre(getDimFlotte('h'), 2 * (getDimFlotte('w')), sx, sy)
{
    navire = listedesnavires();
    initDim();
    for (int i = 0; i < 5; i++)
    {
        color[i] = WBLACK;
        estauport[i] = true;
        s[i] = ' ';
    }
    if (!partiechargee)
    {
        couleursNavires();
        refreshPort(d);
    }
}

Flotte::~Flotte()
{
    for (int i = 0; i < 5; i++)
    {
        delete[] dimNavire[i];
        delete[] dimNavireOriginales[i];
    }
    delete[] dimNavire;
    delete[] dimNavireOriginales;
}

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
        x += 1 + dimNavireOriginales[i][0];
    }

    int fx = x + dimNavireOriginales[n][0];

    while (x < fx)
    {
        for (int y = 0; y < dimNavireOriginales[n][1]; y++)
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
    return -1;
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
    return -1;
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
        x += 1 + dimNavireOriginales[i][0];
    }
    int fx = x + dimNavireOriginales[n][0];

    while (x < fx)
    {
        for (int y = 0; y < dimNavireOriginales[n][1]; y++)
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

            while (x < dimNavireOriginales[n][0])
            {
                for (y = 0; y < dimNavireOriginales[n][1]; y++)
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
            x2 += dimNavireOriginales[n][0];
        }

        x2++;
        x = 0;
        if (delai == 1)
            usleep(60000);
    }
}

bool Flotte::portVide()
{
    for (int n = 0; n < 5; n++)
    {
        if (estauport[n])
        {
            return false;
        }
    }
    return true;
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
    return -1;
}

int Flotte::getDernierNavire()
{
    for (int n = 4; n >= 0; n--)
    {
        if (estauport[n])
        {
            return n;
        }
    }
    return -1;
}

int Flotte::getRandomNavire()
{
    for (int i = 0; i < 5; i++)
    {
        s[i] = ' ';
    }
    refreshPort(0);
    if (portVide())
    {
        return -1;
    }
    int n = rand() % 5;

    while (!getEstAuPort(n))
    {
        n = rand() % 5;
    }
    s[n] = '#';
    refreshPort(0);
    return n;
}

void Flotte::initSelection()
{
    s[getPremierNavire()] = '#';
    refreshPort(0);
}

int Flotte::getHeightnavire(int n)
{

    return dimNavire[n][1];
}

int Flotte::getWidthnavire(int n)
{
    return dimNavire[n][0];
}

int Flotte::getHeightnavireOriginale(int n)
{

    return dimNavireOriginales[n][1];
}

int Flotte::getWidthnavireOriginale(int n)
{
    return dimNavireOriginales[n][0];
}

void Flotte::swapDimensionsNavire(int n)
{
    swap(dimNavire[n][0], dimNavire[n][1]);
}

void Flotte::initDim()
{

    int maxX = 0;
    int maxY = 0;

    dimNavire = new int *[5];
    dimNavireOriginales = new int *[5];

    for (int n = 0; n < 5; n++)
    {
        dimNavire[n] = new int[2];
        dimNavireOriginales[n] = new int[2];

        for (int x = 0; navire[n][x][0] != -1; x++)
        {
            for (int y = 0; navire[n][0][y] != -1; y++)
            {
                if (navire[n][x][y] == 1)
                {
                    maxY = fmax(maxY, y + 1);
                }
                if (navire[n][y][x] == 1)
                {
                    maxX = fmax(maxX, y + 1);
                }
            }
        }

        dimNavire[n][0] = maxX;
        dimNavire[n][1] = maxY;
        dimNavireOriginales[n][0] = maxX;
        dimNavireOriginales[n][1] = maxY;
        maxX = 0;
        maxY = 0;
    }
}

void Flotte::couleursNavires()
{
    ifstream couleurs("config/couleurs.txt", ios::in);
    int lignes = 1;
    string ligne;

    while (getline(couleurs, ligne))
    {
        switch (lignes)
        {
        case 3:
            color[lignes - 3] = convertColor(ligne);
            break;
        case 4:
            color[lignes - 3] = convertColor(ligne);
            break;
        case 5:
            color[lignes - 3] = convertColor(ligne);
            break;
        case 6:
            color[lignes - 3] = convertColor(ligne);
            break;
        case 7:
            color[lignes - 3] = convertColor(ligne);
            break;
        case 9:
            fenetre.setCouleurBordure(convertColor(ligne));
            break;
        case 10:
            fenetre.setCarBordure(ligne[0]);
            couleurs.close();

            return;
        }

        lignes++;
    }
    couleurs.close();
}

void Flotte::chargerDim(int **dim)
{
    dimNavire = dim;
}