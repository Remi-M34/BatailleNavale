#include "../include/message.h"
#include "../include/window.h"
#include "../include/config.h"
#include "../include/grille.h"

#include <fstream>

using namespace std;

string insPlacement()
{
    string message = "Placez vos navires sur la grille \n Entrée    Sélectionner / placer \n Espace    Pivoter \n b         Renvoyer le navire au port";

    return message;
}

string aide()
{

    string message =
        "Le but du jeu est de localiser et détruire les 5 navires de chaque adversaire.\n\nLes joueurs commencent par placer chacun de leur navires sur leur territoire.\nLe jeu débute quand tous les participants ont placé tous leur navires.\n\nUn tirage au sort est effectué pour désigner qui jouera en premier. Après ça, les joueurs peuvent, chacun leur tour, choisir une cible et une case à attaquer.\n\nLe nombre de navires restants de chaque joueur est affiché sur la bordure de leur grille, à droite de leur pseudo.\n\nLe gagnant est le dernier survivant. Son score est alors enregistré et figure peut-être dans le top 10.\nScores :\n((missilesGagnants / missilesTires) * (12500)) / (tailleFlotte / (hauteurGrille * largeurGrille))";

    return message;
}

string aPropos()
{
    string message = "Rémi Mattei\t|  Guillaume Kostrzewski\nRémi Lagardère |  Mohamed Bouchengour";

    return message;
}

string version()
{
    string message = "v. 1.12";

    return message;
}

string infoFlotte(int s)
{
    switch (s)
    {
    case 1:
        return "--- Flotte 1 ---";

    case 2:
        return "--- Flotte 2 ---";
    case 3:
        return "--- Flotte 3 ---";
    case 4:
        return "--- Flotte 4 ---";
    case 5:
        return "--- Flotte 5 ---";
    case 6:
        return "--- Flotte 6 ---";
    case 7:
        return "--- Flotte 7 ---";
    case 8:
        return "--- Flotte 8 ---";
    case 9:
        return "--- Flotte 9 ---";
    case 10:
        return "--- Flotte 10 ---";
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
        return "Moyen";
    case 4:
        return "Moyen";
    case 5:
        return "Moyen";
    case 6:
        return "Moyen";
    case 7:
        return "Difficile";
    case 8:
        return "Difficile";
    case 9:
        return "Difficile";
    case 10:
        return "Difficile";
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


string mauvaiseTaille()
{
    string message;
    message = "La taille doit être comprise entre 10 et 20.";

    return message;
}

string mauvaiseTaille2()
{
    string message;
    message = "La grille fait désormais";

    return message;
}



string dimGrille()
{
    string largeur, hauteur;
    largeur = myitoa(getWidthGrille());
    hauteur = myitoa(getHeightGrille());
    string message = largeur + "x" + hauteur;

    return message;
}


string erreurTailleFenetre()
{

    string largeur, hauteur;
    largeur = myitoa(COLS);
    hauteur = myitoa(LINES);
    string message = "Votre fenêtre est trop petite : " + largeur + " x " + hauteur 
    + "\nLe jeu nécessite une fenêtre de taille supérieure à 150 x 40 pour l'affichage du menu."
    + "\n\nNous recommendons d'utiliser le mode plein écran."
    + "\n\nAppuyez sur une touche pour quitter...";


    return message;
}


string erreurTailleFenetre2()
{
    string message = "Votre fenêtre semble être trop petite pour la configuration choisie.\nAppuyez sur une touche pour continuer malgré tout...";


    return message;
}