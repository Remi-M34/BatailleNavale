#include <iostream>
#include <string>
#include <fstream>
#include "config.h"
#include "flotte.h"
#include <math.h>
#include <stdlib.h>
#include <sstream>
using namespace std;

#define xm (COLS - 100) / 2
#define ym (LINES - 33) / 2

int const getDimFlotte(char c)
{
    ifstream config("config.txt", ios::in);
    string ligne;
    int n = 0;
    int lignes = 0;
    int height[5] = {0};
    int width[5] = {0};
    int longueur = 4;
    int maxheight = 0;

    while (getline(config, ligne))
    {
        // On passe au navire suivant si la ligne ne contient qu'un retour à la ligne
        if (ligne[0] == '\0' && ligne[0] != ' ' && ligne[0] != 'x' && n < 5)
        {
            maxheight = fmax(maxheight, height[n]);
            n++;
            lignes++;
            continue;
        }
        if (lignes > 0)
        {
            for (int i = 0; ligne[i] != '\0'; i++)
            {

                if (i >= width[n])
                {
                    width[n]++;
                }
            }
            height[n]++;
        }
        lignes++;
        if (config.eof() && height[4] == 1)
        {
            width[4]++;
        }
    }

    maxheight = fmax(maxheight, height[n]);

    config.close();

    // Obtention de la longeur cumulée des navires
    for (n = 0; n < 5; n++)
    {
        longueur += width[n];
    }

    if (c == 'w')
        return longueur;
    else
    {
        return maxheight;
    }
}

void editWidthGrille(int w)
{
    fstream tmp("tmp.txt", ios::in | ios::out | ios::trunc);
    tmp << w << "x" << getHeightGrille() << '\n';

    string ligne;
    int lignes = 0;
    fstream config("config.txt", ios::in | ios::out);

    while (getline(config, ligne))
    {
        lignes++;
        if (lignes == 1)
        {
            continue;
        }
        tmp << ligne << '\n';
    }

    rename("tmp.txt", "config.txt");

    tmp.close();
    config.close();
}

void editHeightGrille(int h)
{
    fstream tmp("tmp.txt", ios::in | ios::out | ios::trunc);
    tmp << getWidthGrille() << "x" << h << '\n';

    string ligne;
    int lignes = 0;
    fstream config("config.txt", ios::in | ios::out);

    while (getline(config, ligne))
    {
        lignes++;
        if (lignes == 1)
        {
            continue;
        }
        tmp << ligne << '\n';
    }

    rename("tmp.txt", "config.txt");

    tmp.close();
    config.close();
}

int const getWidthGrille()
{

    ifstream config("config.txt", ios::in);
    string ligne;
    int n = 0;

    getline(config, ligne);

    for (int i = 0; isdigit(ligne[i]); i++)
    {
        n = (n * 10) + ligne[i] - '0';
    }

    config.close();
    return n;
}

int const getHeightGrille()
{
    ifstream config("config.txt", ios::in);
    string ligne;
    int n = 0;
    int i = 0;

    getline(config, ligne);

    while (isdigit(ligne[i]))
    {
        i++;
    }
    i++;

    while (isdigit(ligne[i]))
    {
        n = (n * 10) + ligne[i] - '0';
        i++;
    }

    config.close();

    return n;
}

int **dimN()
{
    ifstream config("config.txt", ios::in);
    string ligne;
    int n = 0;
    int lignes = 0;
    // Dim [n][i] où n est le numero du navire est i est la longueur si 0, hauteur si 1
    int **dim;
    dim = new int *[5];
    for (int i = 0; i < 5; i++)
    {
        dim[i] = new int[2];
    }

    // Obtention des dimensions de chaque navire
    while (getline(config, ligne))
    {

        if (ligne[1] == '\0' && ligne[0] != ' ' && ligne[0] != 'x')
        {
            n++;
            lignes++;

            continue;
        }
        if (lignes > 0)
        {
            for (int i = 0; ligne[i] != '\0'; i++)
            {

                if (i > dim[n][0])
                {
                    dim[n][0]++;
                }
            }

            dim[n][1]++;
        }
        lignes++;
        if (config.eof() && dim[4][1] == 1)
        {
            dim[4][0]++;
        }
    }

    config.close();

    return dim;
}

int ***listedesnavires()
{
    ifstream config("config.txt", ios::in);
    string ligne;
    int n = 0;
    int lignes = 0;
    int height[5] = {0};
    int width[5] = {0};

    // Obtention des dimensions de chaque navire
    while (getline(config, ligne))
    {

        if (ligne[0] == '\0' && ligne[0] != ' ' && ligne[0] != 'x')
        {
            n++;
            lignes++;

            continue;
        }
        if (lignes > 0)
        {
            for (int i = 0; ligne[i] != '\0'; i++)
            {

                if (i >= width[n])
                {
                    width[n]++;
                }
            }

            height[n]++;
        }
        lignes++;
        if (config.eof() && height[4] == 1)
        {
            width[4]++;
        }
    }

    int height2[5];
    int width2[5];

    for (int i = 0; i < 5; i++)
    {
        height2[i] = fmax(height[i], width[i]);
        width2[i] = fmax(height[i], width[i]);
    }

    // Retour au début du fichier config
    config.clear();
    config.seekg(0, ios::beg);

    // Initialisation des navires
    int ***navire;
    navire = new int **[5];
    for (n = 0; n < 5; ++n)
    {
        navire[n] = new int *[width2[n] + 1];
        for (int x = 0; x <= width2[n]; x++)
        {
            navire[n][x] = new int[height2[n] + 1];
            for (int y = 0; y <= height2[n]; y++)
            {
                if (x == width2[n])
                {
                    navire[n][x][y] = -1;
                }
                else if (y == height2[n])
                {
                    navire[n][x][y] = -1;
                }
                else
                {
                    navire[n][x][y] = 0;
                }
            }
        }
    }

    n = 0;
    lignes = 0;
    int x = 0, y = 0;

    // Initialisation des navires - suite
    while (getline(config, ligne))
    {
        if (lignes > 0)
        {
            if (ligne[0] == '\0' && ligne[0] != ' ' && ligne[0] != 'x')
            {
                n++;
                y = 0;
                continue;
            }
            for (int i = 0; i < width[n]; i++)
            {
                if (ligne[i] == 'x')
                {
                    navire[n][i][y] = 1;
                }
            }
            y++;
        }
        lignes++;
        ligne = "     ";
    }

    config.close();

    // for (n = 0; n < 5; n++)
    // {
    //     for (y = 0; y < height[n] + 1; y++)
    //     {
    //         for (x = 0; x < width[n] + 1; x++)
    //         {
    //             cout << navire[n][x][y];
    //         }
    //         cout << endl;
    //     }
    // }
    // cout << width[4] << endl;
    //Affichage de toutes les cases des navires

    return navire;
}

void modifierNavires(int n, int width, int height, int **nouveaunavire)
{

    int lignes = -1;
    string ligne;
    int navire = 1;
    bool trouve = false;
    bool saut = false;

    fstream tmp0("tmp.txt", ios::in | ios::out | ios::trunc);
    tmp0 << "15x15\n";
    tmp0.close();

    for (int i = 0; i != n; i++)
    {
        ecrireNavire(i);
    }

    fstream tmp("tmp.txt", ios::in | ios::out | ios::app);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (nouveaunavire[x][y] == 0)
            {
                tmp << ' ';
            }
            else if (nouveaunavire[x][y] == 1)
            {
                tmp << 'x';
            }
        }
        tmp << '\n';
    }
    if (n != 4)
        tmp << '\n';
    tmp.close();

    if (n != 4)
    {
        for (int i = n + 1; i < 5; i++)
        {
            ecrireNavire(i);
        }
    }

    rename("tmp.txt", "config.txt");
}

void ecrireNavire(int n)
{
    fstream config("config.txt", ios::in | ios::out);

    fstream tmp("tmp.txt", ios::in | ios::out | ios::app);

    int lignes = 0;
    string ligne;
    int navire = 0;

    while (getline(config, ligne))
    {
        lignes++;
        if (lignes == 1)
        {
            // if (n == 0)
            // {
            //     continue;
            // }
            continue;
        }

        if (navire == n)
        {
            if (ligne[0] != ' ' && ligne[0] != 'x')
            {
                if (!config.eof())
                {
                    tmp << '\n';
                }
                config.close();
                tmp.close();
                return;
            }
            tmp << ligne;
            if (!config.eof())
            {
                tmp << '\n';
            }
            continue;
        }

        if (navire != n)
        {
            if (ligne[0] != ' ' && ligne[0] != 'x')
            {
                navire++;
            }
            continue;
        }
    }

    config.close();
    tmp.close();
}

void changerDim()
{
    stopProgramX();
    startProgramX();

    int ch;
    int n = 0;

    Window plateau(33, 100, xm, ym, WMAGENTA);

    Window dim(3, 18, 41 + xm, 13 + ym, ' ');
    plateau.setBordureDroite();
    dim.setCouleurBordure(BGREEN);
    dim.setCouleurFenetre(WBLACK);
    Flotte flotte(50 - getDimFlotte('w') + xm, 4 + ym, 0);

    Window aide(8, 30, 2 + xm, 11 + ym);
    aide.setBordureDroite();
    aide.print(1, 1, "<-, ->     Parcourir", WBLACK);
    aide.print(1, 2, "ENTREE     Sélectionner", WBLACK);

    aide.print(1, 7, "q          Menu", WBLACK);

    echo();

    int h = getHeightGrille();
    int w = getWidthGrille();
    string height;
    string width;
    ostringstream convert;
    convert << h;
    height = convert.str();
    ostringstream convert2;
    convert2 << w;
    width = convert2.str();

    int x = 0;
    int num = 0;
    Color col[3] = {BWHITE, BMAGENTA, BMAGENTA};
    dim.print(5 + width.length() + 2, 1, "x", WBLACK);

    while ((ch = getch()) != 'q')
    {
        string retour = "Retour";
        echo();

        dim.print(5, 1, width, col[0]);
        dim.print(5 + width.length() + 5, 1, height, col[1]);
        plateau.print(50 - retour.length() / 2, 18, retour, col[2]);

        switch (ch)
        {
        case KEY_RIGHT:
            if (n == 0)
            {
                n++;
                swap(col[0], col[1]);
            }
            break;

        case KEY_LEFT:
            if (n == 1)
            {
                n--;
                swap(col[0], col[1]);
            }
            break;

        case KEY_DOWN:
            if (n != 2)
            {
                swap(col[n], col[2]);
                n = 2;
            }
            break;

        case KEY_UP:
            if (n == 2)
            {
                swap(col[0], col[2]);
                n = 0;
            }
            break;

        case '\n':
            aide.print(1, 4, "1-9        Modifier la taille", WBLACK);
            aide.print(1, 5, "ENTREE     Valider", WBLACK);
            curs_set(1);

            if (n == 0)
            {
                echo();
                for (int i = 0; i < width.length(); i++)
                {
                    dim.print(5 + i, 1, ' ', WBLACK);
                }

                move(15 + ym, 47 + xm);

                do
                {

                    ch = getch();
                    if (isdigit(ch))
                    {

                        // cout << ch  ;
                        num = (num * 10) + ch - '0';
                    }
                    if (ch == '\n' && num > 0)
                    {
                        editWidthGrille(num);
                        changerDim();
                        return;

                        break;
                    }
                    else if (ch == '\n' && num == 0)
                    {
                        move(15 + ym, 47 + xm);
                    }
                    if (isalpha(ch))
                    {
                        changerDim();
                        return;
                    }
                } while (1);
            }
            else if (n == 1)
            {

                echo();
                for (int i = 0; i < height.length(); i++)
                {
                    dim.print(10 + i + width.length(), 1, ' ', WBLACK);
                }

                move(15 + ym, xm + 52 + width.length());

                do
                {

                    ch = getch();
                    if (isdigit(ch))
                    {
                        num = (num * 10) + ch - '0';
                    }
                    if (ch == '\n' && num > 0)
                    {
                        editHeightGrille(num);
                        changerDim();

                        return;
                        break;
                    }
                    else if (ch == '\n' && num == 0)
                    {
                        move(15 + ym, xm + 52 + width.length());
                    }
                } while (1);
            }
            else if (n == 2)
            {
                curs_set(0);

                return;
            }
            break;
        }
    }
}

Color convertColor(string ligne)
{

    if (!ligne.find("WBLACK"))
        return WBLACK;
    else if (!ligne.find("WCYAN"))
        return WCYAN;
    else if (!ligne.find("WBLUE"))
        return WBLUE;
    else if (!ligne.find("WYELLOW"))
        return WYELLOW;
    else if (!ligne.find("WGREEN"))
        return WGREEN;
    else if (!ligne.find("WMAGENTA"))
        return WMAGENTA;
    else if (!ligne.find("WRED"))
        return WRED;
    else if (!ligne.find("BWHITE"))
        return BWHITE;
    else if (!ligne.find("BCYAN"))
        return BCYAN;
    else if (!ligne.find("BBLUE"))
        return BBLUE;
    else if (!ligne.find("BYELLOW"))
        return BYELLOW;
    else if (!ligne.find("BGREEN"))
        return BGREEN;
    else if (!ligne.find("BMAGENTA"))
        return BMAGENTA;
    else if (!ligne.find("BRED"))
        return BRED;
    
    return WBLACK;
}

