#include <iostream>
#include <string>
#include <fstream>
#include "config.h"
using namespace std;




 int const getWidthFlotte()
{
    ifstream config("config.txt", ios::in);
    string ligne;
    int n = 0;
    int lignes = 0;
    int height[5] = {0};
    int width[5] = {0};
        int longueur = 4;


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

                if (i > width[n])
                {
                    width[n]++;
                }
            }
            height[n]++;
        }
        lignes++;
    }

        config.close();


// Obtention de la longeur cumulée des navires
    for (n = 0 ; n < 5 ; n++)
    {
        longueur+=width[n];
    }

    return longueur;
    }





 int const getHeightGrille()
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






 int const getWidthGrille()
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




int ***listedesnavires()
{
    ifstream config("config.txt", ios::in);
    string ligne;
    int n = 0;
    int lignes = 0;
    int height[5] = {0};
    int width[5] = {0};


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

                if (i > width[n])
                {
                    width[n]++;
                }
            }
            height[n]++;
        }
        lignes++;
    }






    // Retour au début du fichier config
    config.clear();
    config.seekg(0, ios::beg);

    // Initialisation des navires
    int ***navire;
    navire = new int **[5];
    for (n = 0; n < 5; ++n)
    {
        navire[n] = new int *[width[n] + 1];
        for (int x = 0; x <= width[n]; x++)
        {
            navire[n][x] = new int[height[n] + 1];
            for (int y = 0; y <= height[n]; y++)
            {
                if (x == width[n])
                {
                    navire[n][x][y] = -1;
                }
                else if (y == height[n])
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
            if (ligne[1] == '\0' && ligne[0] != ' ' && ligne[0] != 'x')
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
    }

    config.close();

//            for ( n = 0; n < 5; n++)
//    {
//        for ( x = 0; x < height[n]; x++)
//        {
//            for ( y = 0; y < width[n]; y++)
//            {
//                cout << navire[n][y][x];
//            }
//            cout << endl;
//}
 //   }

    //Affichage de toutes les cases des navires


    return navire;
}
