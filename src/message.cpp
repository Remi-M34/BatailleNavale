#include "../include/headers.h"

using namespace std;

string insPlacement()
{
    string message = "Placez vos navires sur la grille \n Entrée    Sélectionner / placer \n Espace    Pivoter \n b         Renvoyer le navire au port";

    return message;
}

string aide()
{

    string message =
        "Le but du jeu est d'être le premier joueur à avoir situé tous les navires ennemis sur le terrain maritime de l'adversaire.\n\nIl se déroule en 2 phases :\nLe joueur commence par sélectionner un de ses navires pour ensuite le placer où il le veut sur son terrain.\nSélection et validation du navire avec ENTREE, rotation avec ESPACE\nLe jeu débute quand les deux joueurs ont placé tous leur navires.\n\nUn premier joueur (désigné aléatoirement) peut envoyer un missile sur une des cases du plateau ennemi :\n- Une partie d'un navire est touchée, le joueur peut rejouer et gagne x points (x si le navire est coulé)\n- Le missile tombe à l'eau : c'est au tour de l'adversaire de jouer\nLe jeu se termine quand un joueur a coulé tous les navires ennemis.";

    return message;
}

string aPropos()
{
    string message = "Rémi M - Guillaume K - Rémi L";

    return message;
}

string version()
{
    string message = "v. 0.31";

    return message;
}

string infoFlotte(int s)
{
    switch (s)
    {
    case 1:
        return "--- Thème 1 ---";

    case 2:
        return "--- Thème 2 ---";
    case 3:
        return "--- Thème 3 ---";
    case 4:
        return "--- Thème 4 ---";
    case 5:
        return "--- Thème 5 ---";
    case 6:
        return "--- Thème 6 ---";
    case 7:
        return "--- Thème 7 ---";
    case 8:
        return "--- Thème 8 ---";
    case 9:
        return "--- Thème 9 ---";
    case 10:
        return "--- Thème 10 ---";
    }
}

string infoFlotteD(int s)
{
    switch (s)
    {
    case 1:
        return "Facile";
    case 2:
        return "Facile";
    case 3:
        return "Facile";
    case 4:
        return "Facile";
    case 5:
        return "Facile";
    case 6:
        return "Facile";
    case 7:
        return "Facile";
    case 8:
        return "Facile";
    case 9:
        return "Facile";
    case 10:
        return "Facile";
    }
}

string infoFlotteT()
{
    fstream config("config/config.txt", ios::in);
    string ligne;
    getline(config, ligne);

    config.close();

        return ("Grille : " + ligne);
}