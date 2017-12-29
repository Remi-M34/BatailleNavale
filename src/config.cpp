#include "../include/window.h"
#include "../include/navire.h"
#include "../include/config.h"
#include "../include/message.h"


#include <fstream>
#include <unistd.h>
#include <cmath>
#include <cassert>
#include <stdlib.h>


using namespace std;

#define xm (COLS - 100) / 2
#define ym (LINES - 33) / 2

int const getDimFlotte(char c)
{
    ifstream config("config/config.txt", ios::in);
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
    if (w > 20)
    {
        w = 20;
    }
    else if (w < 10)
    {
        w = 10;
    }
    fstream tmp("config/tmp.txt", ios::in | ios::out | ios::trunc);
    tmp << w << "x" << getHeightGrille() << '\n';

    string ligne;
    int lignes = 0;
    fstream config("config/config.txt", ios::in | ios::out);

    while (getline(config, ligne))
    {
        lignes++;
        if (lignes == 1)
        {
            continue;
        }
        tmp << ligne << '\n';
    }

    rename("config/tmp.txt", "config/config.txt");

    tmp.close();
    config.close();
}

void editHeightGrille(int h)
{
        if (h > 20)
    {
        h = 20;
    }
    else if (h < 10)
    {
        h = 10;
    }
    fstream tmp("config/tmp.txt", ios::in | ios::out | ios::trunc);
    tmp << getWidthGrille() << "x" << h << '\n';

    string ligne;
    int lignes = 0;
    fstream config("config/config.txt", ios::in | ios::out);

    while (getline(config, ligne))
    {
        lignes++;
        if (lignes == 1)
        {
            continue;
        }
        tmp << ligne << '\n';
    }

    rename("config/tmp.txt", "config/config.txt");

    tmp.close();
    config.close();
}

int const getWidthGrille()
{

    ifstream config("config/config.txt", ios::in);
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
    ifstream config("config/config.txt", ios::in);
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
    ifstream config("config/config.txt", ios::in);
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
    ifstream config("config/config.txt", ios::in);
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

    fstream tmp0("config/tmp.txt", ios::in | ios::out | ios::trunc);
    tmp0 << getWidthGrille() << "x" << getHeightGrille() << '\n';
    tmp0.close();

    for (int i = 0; i != n; i++)
    {
        ecrireNavire(i);
    }

    fstream tmp("config/tmp.txt", ios::in | ios::out | ios::app);
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

    rename("config/tmp.txt", "config/config.txt");
}

void ecrireNavire(int n)
{
    fstream config("config/config.txt", ios::in | ios::out);

    fstream tmp("config/tmp.txt", ios::in | ios::out | ios::app);

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

void changeTheme(string th)
{

    ofstream stream2("config/couleurs.txt");

    if (th == "Defaut")
    {
        ifstream stream1("config/themes/Defaut");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (th == "Theme2")
    {
        ifstream stream1("config/themes/theme2");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (th == "Theme3")
    {
        ifstream stream1("config/themes/theme3");
        stream2 << stream1.rdbuf();
        stream1.close();
    }

    else if (th == "Theme4")
    {
        ifstream stream1("config/themes/theme4");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    stream2.close();
}

void changePreset(int n)
{
    ofstream stream2("config/config.txt");

    if (n == 1)
    {
        ifstream stream1("config/preset/1");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (n == 2)
    {
        ifstream stream1("config/preset/2");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (n == 3)
    {
        ifstream stream1("config/preset/3");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (n == 4)
    {
        ifstream stream1("config/preset/4");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (n == 5)
    {
        ifstream stream1("config/preset/5");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (n == 6)
    {
        ifstream stream1("config/preset/6");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (n == 7)
    {
        ifstream stream1("config/preset/7");
        stream2 << stream1.rdbuf();
        stream1.close();
    }

    else if (n == 8)
    {
        ifstream stream1("config/preset/8");
        stream2 << stream1.rdbuf();
        stream1.close();
    }
    else if (n == 9)
    {
        ifstream stream1("config/preset/9");
        stream2 << stream1.rdbuf();
        stream1.close();
    }

    stream2.close();
}

// Convertie un string en int (cppreference)

std::string to_string(int x)
{
    int length = snprintf(NULL, 0, "%d", x);
    assert(length >= 0);
    char *buf = new char[length + 1];
    snprintf(buf, length + 1, "%d", x);
    std::string str(buf);
    delete[] buf;
    return str;
}




void erreurEcran(int w, int h)
{
  halfdelay(1);
    
  Window erreur(LINES, COLS, 0-COLS/2, 0-LINES/2, ' ',false);
  while (COLS < w || LINES < h)
  {
    erreur.print(0, 1, erreurTailleFenetre());
    usleep(3000000);


      }

  Window m(LINES, COLS, 0-25, 0, ' ',false);

  erase();
  int ch;
  while ((ch = getch()) != '\n')
  {

    // cin.ignore(99999,'\n');
    m.print(1, 1, "Taille OK! Appuyez sur ENTREE pour démarrer le jeu.");
    // usleep(50000);
  }
  erase();

    stopProgramX();

  startProgramX();
}

