#include "../include/menu.h"
#include "../include/window.h"
#include "../include/flotte.h"
#include "../include/config.h"
#include "../include/jeu.h"
#include "../include/message.h"
#include "../include/grille.h"
#include "../include/navire.h"

#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <ctime>

#define H getHeightGrille()
#define W getWidthGrille()

using namespace std;

Menu::Menu() : plateau(33, 100, -50, -16, carBordureMenu)
{
  nbjoueurs = 0;
  nbjoueurshumain = 0;
  difficulte = 2;
  vitesse = 6;
  initCouleurs();
}

Menu::~Menu() {}

void Menu::initCouleurs()
{
  ifstream couleurs("config/couleurs.txt", ios::in);
  int lignes = 1;
  string ligne;

  while (getline(couleurs, ligne))
  {

    switch (lignes)
    {
    case 9:
      colBordureFlotte = convertColor(ligne);
      break;
    case 42:
      colSelectionnee = convertColor(ligne);
      break;
    case 43:
      colNonSelectionnee = convertColor(ligne);
      break;
    case 44:
      if (ligne[0] == 'o' || ligne[0] == 'O')
      {
        bordureMenuFine = true;
      }
      break;
    case 45:
      colBordureMenu = convertColor(ligne);
      break;
    case 46:
      carBordureMenu = ligne[0];
      break;
    case 50:
      theme.clear();
      theme = ligne;
      couleurs.close();
      return;
    }
    lignes++;
  }

  couleurs.close();
}

void Menu::mainMenu(int d)
{
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuFine)
  {
    plateau.setBordureFine();
  }

  Window apropos(3, 40, 50 - 40 / 2 + -50, 29 + -16, ' ');
  apropos.setCouleurBordure(WBLACK);

  Color col[8];
  col[0] = colSelectionnee;
  for (int i = 1; i < 8; i++)
  {
    col[i] = colNonSelectionnee;
  }

  refresh();
  getch();

  int c;
  string choix[8] = {"Commencer une partie", "Continuer...", "Top Scores", "Options", "Aide", "Version du jeu", "A Propos", "Quitter"};
  int selection = 0;
  Window top(16, 25, -48, -3);
  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, d, false);
  while ((c = getch()))
  {
    if (selection < 5)
    {
      apropos.clearall();
    }
    if (selection != 2)
    {
      top.clearall();
    }

    for (int i = 0; i < 8; i++)
    {
      plateau.print(50 - choix[i].length() / 2, 12 + 2 * i + (i == 7 ? 1 : 0), choix[i], col[i]);
    }
    plateau.print(50 - choix[selection].length() / 2 - 2, 12 + 2 * selection + (selection == 7 ? 1 : 0), '>', WBLACK);

    switch (c)
    {
    case KEY_UP:
      if (selection != 0)
      {
        swap(col[selection], col[selection - 1]);
        plateau.print(50 - choix[selection].length() / 2 - 2, 12 + 2 * selection + (selection == 7 ? 1 : 0), ' ', WBLACK);

        selection--;
      }
      break;

    case KEY_DOWN:
      if (selection != 7)
      {
        swap(col[selection], col[selection + 1]);
        plateau.print(50 - choix[selection].length() / 2 - 2, 12 + 2 * selection, ' ', WBLACK);

        selection++;
      }

      break;
    case '\n':
      if (selection == 0)
      {
        checkTailleEcran();
        // refreshMenu();
        flotte.fenetre.update();
        // flotte.refreshPort(0);
        selectJoueurs(plateau);
        if (nbjoueurs > 0)
        {
          optionsJeu();
          Jeu Jeu(nbjoueurs, nbjoueurshumain, difficulte, vitesse, nom, log, false);
          Jeu.Phase1();
        }
        return mainMenu(0);
      }
      else if (selection == 1)
      {
        chargement();
        refreshMenu();
        flotte.refreshPort(0);
        flotte.fenetre.setCouleurBordure(colBordureFlotte);
      }
      else if (selection == 2)
      {
        topscores(top);
      }
      else if (selection == 3)
      {
        plateau.clear();
        options();
        refreshMenu();
        flotte.fenetre.setCouleurBordure(colBordureFlotte);
        flotte.refreshPort(0);
      }
      else if (selection == 4)
      {
        aideMenu();
      }
      else if (selection == 5)
      {
        apropos.clear();
        apropos.print(17, 1, version());
        apropos.setBordureFine();
      }
      else if (selection == 6)
      {
        apropos.clear();
        apropos.print(0, 0, aPropos());
        apropos.setBordureFine();
      }
      else if (selection == 7)
      {
        stopProgramX();
        exit(1);
      }
      break;
    }
  }
}

void Menu::refreshMenu()
{
  plateau.clear();
  plateau.update();
  if (bordureMenuFine)
  {
    plateau.setBordureFine();
  }
  else
  {
    plateau.setCouleurBordure(colBordureMenu);
  }

}

void Menu::selectJoueurs(Window plateau)
{
  int c;
  int selection = 0;

  Color col[5] = {colSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee};

  while (c = getch())
  {
    for (int i = 0; i < 5; i++)
    {
      plateau.print(66, 12 + 2 * i, myitoa(i + 2) + " Joueurs", col[i]);
    }
    plateau.print(64, 12 + 2 * selection, '>', WBLACK);

    switch (c)
    {
    case KEY_DOWN:
      if (selection < 4)
      {
        swap(col[selection], col[selection + 1]);
        plateau.print(64, 12 + 2 * selection, ' ', WBLACK);

        selection++;
      }
      break;
    case KEY_UP:
      if (selection > 0)
      {
        swap(col[selection], col[selection - 1]);
        plateau.print(64, 12 + 2 * selection, ' ', WBLACK);

        selection--;
      }
      break;
    case KEY_LEFT:
      plateau.clear();
      mainMenu(0);
      return;
    case '\n':
      nbjoueurs = selection + 2;
      return;
    }
  }
  return;
}

// Menu  des options
void Menu::options()
{

  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuFine)
  {
    plateau.setBordureFine();
  }

  Color col[5] = {colSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee};

  refresh();
  getch();

  int c;
  string choix[5] = {"Editeur de navires", "Modifier la taille de la grille", "Thèmes", "Charger flotte", "Retour"};
  int selection = 0;

  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0, false);

  for (int i = 0; i < 5; i++)
  {
    plateau.print(50 - choix[i].length() / 2, 12 + 2 * i, choix[i], col[i]);
  }

  while ((c = getch()) != 'q')
  {

    for (int i = 0; i < 5; i++)
    {
      plateau.print(50 - choix[i].length() / 2, 12 + 2 * i, choix[i], col[i]);
    }

    plateau.print(50 - choix[selection].length() / 2 - 2, 12 + 2 * selection, '>', WBLACK);

    switch (c)
    {
    case KEY_UP:
      if (selection != 0)
      {
        swap(col[selection], col[selection - 1]);
        plateau.print(50 - choix[selection].length() / 2 - 2, 12 + 2 * selection, ' ', WBLACK);

        selection--;
      }
      break;

    case KEY_DOWN:
      if (selection != 4)
      {
        swap(col[selection], col[selection + 1]);
        plateau.print(50 - choix[selection].length() / 2 - 2, 12 + 2 * selection, ' ', WBLACK);

        selection++;
      }

      break;

    case '\n':
      if (selection == 0)
      {
        menuNavire();
        plateau.clearall();
        return mainMenu(1);
      }
      else if (selection == 1)
      {
        plateau.clear();
        refreshMenu();
        changerDim();
        refreshMenu();
        flotte.fenetre.setCouleurBordure(colBordureFlotte);
        flotte.refreshPort(0);
      }
      else if (selection == 2)
      {
        refreshMenu();
        themes();
        initCouleurs();
        refreshMenu();

        return mainMenu(0);
        flotte.fenetre.setCouleurBordure(colBordureFlotte);
        flotte.refreshPort(0);
      }
      else if (selection == 3)
      {
        refreshMenu();

        preset(1);
        refreshMenu();

        return mainMenu(0);
      }
      else if (selection == 4)
      {
        return;
      }
      break;
    }
  }

  erase();
}

void Menu::aideMenu()
{
  int ch;
  Window aidef(19, 85, 7 + -50, 12 + -16);
  aidef.setBordureFine();
  aidef.setCouleurFenetre(WBLACK);
  aidef.print(39, 0, "Règles :", BBLUE);

  aidef.print(0, 2, aide(), WBLACK);
  aidef.print(39, 18, "Retour", colSelectionnee);

  while ((ch = getch()) != '\n')
  {
  }
}

void Menu::changerDim()
{
  int ch;
  int n = 0;

  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuFine)
  {
    plateau.setBordureFine();
  }

  Window dim(3, 18, 41 + -50, 13 + -16, ' ');
  dim.setCouleurBordure(BGREEN);
  dim.setCouleurFenetre(WBLACK);
  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0, false);

  Window aide(8, 30, 2 + -50, 11 + -16);
  aide.setBordureFine();
  aide.print(1, 1, "<-, ->     Parcourir", WBLACK);
  aide.print(1, 2, "ENTREE     Sélectionner", WBLACK);
  aide.print(1, 7, "q          Menu", WBLACK);

  const int h = getHeightGrille();
  const int w = getWidthGrille();
  string height;
  string width;
  ostringstream convert;
  convert << h;
  height = convert.str();
  ostringstream convert2;
  convert2 << w;
  width = convert2.str();

  int num = 0;
  Color col[3] = {colSelectionnee, colNonSelectionnee, colNonSelectionnee};
  dim.print(5 + width.length() + 2, 1, "x", WBLACK);

  while ((ch = getch()) != 'q')
  {
    string retour = "Retour";

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
        for (unsigned int i = 0; i < width.length(); i++)
        {
          dim.print(5 + i, 1, ' ', WBLACK);
        }

        move(15 + LINES / 2 - 16, 47 + COLS / 2 - 50);

        do
        {

          ch = getch();
          if (isdigit(ch))
          {
            num = (num * 10) + ch - '0';
          }
          if (ch == '\n' && num > 0)
          {
            curs_set(0);
            noecho();
            editWidthGrille(num);
            if (num < 10 || num > 20)
            {
              plateau.print((plateau.getWindowWidth() - mauvaiseTaille().length()) / 2, 23, mauvaiseTaille(), BRED);
              plateau.print((plateau.getWindowWidth() - mauvaiseTaille2().length()) / 2, 24, mauvaiseTaille2(), BRED);
              plateau.print(plateau.getWindowWidth() / 2 + mauvaiseTaille2().length() / 2 + 1, 24, dimGrille(), BYELLOW);
              usleep(2650000);
            }
            changerDim();
            return;

            break;
          }
          else if (ch == '\n' && num == 0)
          {
            move(15 + LINES / 2 - 16, 47 + COLS / 2 - 50);
          }
          else if (ch == 'q')
          {
            curs_set(0);
            noecho();
            return;
          }
          else if (!isdigit(ch))
          {
            continue;
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
        for (unsigned int i = 0; i < height.length(); i++)
        {
          dim.print(10 + i + width.length(), 1, ' ', WBLACK);
        }

        move(15 + LINES / 2 - 16, COLS / 2 - 50 + 52 + width.length());

        do
        {

          ch = getch();
          if (isdigit(ch))
          {
            num = (num * 10) + ch - '0';
          }
          if (ch == '\n' && num > 0)
          {
            curs_set(0);
            noecho();
            editHeightGrille(num);
            if (num < 10 || num > 20)
            {
              plateau.print((plateau.getWindowWidth() - mauvaiseTaille().length()) / 2, 23, mauvaiseTaille(), BRED);
              plateau.print((plateau.getWindowWidth() - mauvaiseTaille2().length()) / 2, 24, mauvaiseTaille2(), BRED);
              plateau.print(plateau.getWindowWidth() / 2 + mauvaiseTaille2().length() / 2 + 1, 24, dimGrille(), BYELLOW);
              usleep(2650000);
            }
            changerDim();

            return;
            break;
          }
          else if (ch == 'q')
          {
            curs_set(0);
            noecho();
            return;
          }
          else if (!isdigit(ch))
          {
            continue;
          }
          else if (ch == '\n' && num == 0)
          {
            move(15 + LINES / 2 - 16, COLS / 2 - 50 + 52 + width.length());
          }
        } while (1);
      }
      else if (n == 2)
      {
        noecho();
        curs_set(0);

        return;
      }
      break;
    }
  }
  noecho();
}

void Menu::menuNavire()
{
  int c;
  int n = 0;

  plateau.clear();
  refreshMenu();

  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuFine)
  {
    plateau.setBordureFine();
  }
  Window aide(12, 30, 2 + -50, 11 + -16);
  aide.setBordureFine();
  aide.print(1, 1, "<-, ->     Parcourir", WBLACK);
  aide.print(1, 2, "ENTREE     Sélectionner", WBLACK);
  aide.print(1, 11, "q          Menu", WBLACK);

  Color col = colNonSelectionnee;
  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0, false);

  flotte.initSelection();

  while ((c = getch()) != 'q')
  {
    plateau.print(15, 5, "Menu", col);

    switch (c)
    {
    case KEY_RIGHT:
      if (n == -1)
      {
        flotte.selectionne(0, true);
        flotte.refreshPort(0);
        col = colNonSelectionnee;
        n++;
        break;
      }
      if (n != 4)
      {
        n++;
        flotte.echangeSelection(n, n - 1);
        flotte.refreshPort(0);
      }
      break;

    case KEY_LEFT:
      if (n > 0)
      {
        n--;
        flotte.echangeSelection(n, n + 1);
        flotte.refreshPort(0);
      }
      else if (n == 0)
      {
        n--;
        col = colSelectionnee;
        flotte.selectionne(0, false);
        flotte.refreshPort(0);
      }
      break;
    case '\n':
      if (n == -1)
      {
        return;
      }
      aide.print(1, 4, "ESPACE     Ajouter une case", WBLACK);
      aide.print(1, 5, "ENTREE     Valider", WBLACK);
      aide.print(1, 6, "r          Annuler l'édition", WBLACK);
      aide.print(1, 7, "d          Repositionner", WBLACK);
      flotte.estAuPort(n, false);
      nouveauNavire(n);
      flotte.estAuPort(n, true);
      menuNavire();
      return;

      break;

    case 'q':
      return;

      break;
    }
  }
}

void Menu::themes()
{

  int c;

  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuFine)
  {
    plateau.setBordureFine();
  }

  Color col[6] = {plateau.getCouleurFenetre(), colSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee};

  string th = "Thème actuel: " + theme;
  string choix[6] = {th, "Defaut (recommandé)", "Theme2", "Retour"};

  int selection = 1;

  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0, false);

  while ((c = getch()) != 'q')
  {

    for (int i = 0; i < 4; i++)
    {
      plateau.print(50 - choix[i].length() / 2, 12 + 2 * i, choix[i], col[i]);
    }
    switch (c)
    {
    case KEY_UP:
      if (selection != 1)
      {
        swap(col[selection], col[selection - 1]);
        selection--;
      }
      break;

    case KEY_DOWN:
      if (selection != 3)
      {
        swap(col[selection], col[selection + 1]);
        selection++;
      }
      break;

    case '\n':
      switch (selection)
      {
      case 1:
        theme = "Defaut";
        changeTheme(theme);
        initCouleurs();
        refresh();
        flotte.refreshPort(0);
        return;
      case 2:
        theme = "Theme2";
        changeTheme(theme);
        initCouleurs();
        refresh();
        flotte.refreshPort(0);
        return;
      case 3:
        return;
      }
      if (selection != 5)
      {
      }
      else
      {
        return;
      }
    }
  }
}

void Menu::preset(int s)
{

  int c;

  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuFine)
  {
    plateau.setBordureFine();
  }

  plateau.print(47, 22, "Valider", colSelectionnee);

  Window aide(5, 24, -50 + 50 - 12, -16 + 25);
  aide.setBordureFine();
  aide.print(1, 1, "<-, ->       Parcourir", WBLACK);
  aide.print(1, 2, "ENTREE         Valider", WBLACK);
  aide.print(1, 4, "q                 Menu", WBLACK);

  Window info(6, 30, -50 + 50 - 15, -16 + 3);
  info.setBordureFine();

  int selection = s;

  changePreset(selection);
  Flotte flotte(50 - getDimFlotte('w') + -50, 14 + -16, 0, false);

  info.print(15 - (infoFlotte(s).length()) / 2, 1, infoFlotte(s));
  info.print(15 - (infoFlotteD(s).length()) / 2, 2, infoFlotteD(s));
  info.print(15 - (infoFlotteT().length()) / 2, 5, infoFlotteT());

  while ((c = getch()) != 'q')
  {

    switch (c)
    {
    case KEY_RIGHT:
      if (selection < 9)
      {
        preset(selection + 1);
        return;
      }
      else
      {
        continue;
      }
    case KEY_LEFT:
      if (selection > 1)
      {
        preset(selection - 1);
        return;
      }
      else
      {
        continue;
      }

    case '\n':
      return;
    }
  }
  
  return;
}

void Menu::checkTailleEcran()
{
  if (COLS < 5 * getWidthGrille() || LINES < 2.5 * getHeightGrille())
  {
    Window erreur(2, 90, -45, 14, '+');
    erreur.print(0, 0, erreurTailleFenetre2());
    getchar();
    // assert(COLS > 5 * getWidthGrille() && LINES > 2.5 * getHeightGrille());
  }
}


void Menu::optionsJeu()
{


  this->log = "fichier.log";
  int c;
  nbjoueurshumain = 0;
  Window plateau2(33, 100, -50, -16, carBordureMenu);
  plateau2.setCouleurBordure(colBordureMenu);
  if (bordureMenuFine)
  {
    plateau2.setBordureFine();
  }

  Window aide(6, 40, -50 + 30, 26 + -16, ' ');
  aide.setBordureFine();

  Color col[12];
  for (int i = 0; i < 12; i++)
  {
    col[i] = colNonSelectionnee;
  }
  col[1] = colSelectionnee;

  string choix[12] = {"Vitesse IA: x", "Difficulté: ", "Nom du fichier log: ", "Humains", "Ajouter un joueur", "Ajouter un joueur humain",
                      "Ajouter un joueur humain", "Ajouter un joueur humain",
                      "Ajouter un joueur humain", "Ajouter un joueur humain", "Jouer", "Retour"};
  string difficulte[3] = {"Facile", "Normal", "Difficile"};
  int selection = 1;

  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0, false);
  aideOptionsJeu(aide, selection);

  for (int i = 0; i < nbjoueurs; i++)
  {
    plateau2.print(9, 14 + 2 * i, "     ");

    if (i < nbjoueurs - nbjoueurshumain)
    {
      string str = "Bot " + myitoa(i + 1);
      plateau2.print(9, 14 + 2 * i, str);
    }
  }

  while ((c = getch()))
  {

    plateau2.print(70, 12, choix[0] + myitoa(vitesse), col[0]);
    plateau2.print(41, 12, choix[1] + difficulte[this->difficulte - 1], col[1]);
    plateau2.print(41, 14, choix[2] + log, col[2]);
    plateau2.print(23, 12, choix[3] + " (" + myitoa(nbjoueurshumain) + ")", col[3]);
    plateau2.print(9, 12, "IA (" + myitoa(nbjoueurs - nbjoueurshumain) + ")", BBLUE);
    plateau2.print(48, 24, choix[10], col[10]);
    plateau2.print(1, 24, choix[11], col[11]);

    for (int i = 4; i < nbjoueurshumain + 5 && i < nbjoueurs + 4; i++)
    {

      plateau2.print(23, 14 + 2 * (i - 4), choix[i], col[i]);
    }
    switch (c)
    {
    case KEY_UP:
      if ((selection > 3 && selection < 9) || selection == 2)
      {
        swap(col[selection], col[selection - 1]);
        selection--;
      }
      else if (selection == 0)
      {
        vitesse = min(9, vitesse + 1);
      }
      else if (selection == 10)
      {
        swap(col[selection], col[2]);
        selection = 2;
      }
      aideOptionsJeu(aide, selection);

      break;

    case KEY_DOWN:
      if (selection < nbjoueurshumain + 4 && selection > 0 && selection < 3 + nbjoueurs && selection != 2)
      {
        swap(col[selection], col[selection + 1]);
        selection++;
      }
      else if (selection == 0)
      {
        vitesse = max(0, vitesse - 1);
      }
      else if (selection == 2 || selection == min(nbjoueurs, nbjoueurshumain) + 4 || selection == 3 + nbjoueurs)
      {
        swap(col[selection], col[10]);
        selection = 10;
      }
      aideOptionsJeu(aide, selection);

      break;
    case KEY_RIGHT:
      if (selection > 0 && selection < 4 && selection != 2)
      {
        swap(col[selection], col[selection-1]);
        selection--;
      }
      else if (selection == 4)
      {
        swap(col[selection], col[2]);
        selection = 2;
      }
      else if (selection == 11)
      {
        swap(col[selection], col[selection-1]);
                selection--;

      }
      aideOptionsJeu(aide, selection);

      break;

    case KEY_LEFT:
      if (selection < 3)
      {
        if (selection == 0)
        {
          swap(col[selection], col[selection+1]);
                  selection++;
        }
        else if (selection == 1)
        {
          swap(col[selection], col[3]);
          selection = 3;
        }
        else if (selection == 2)
        {
          swap(col[selection], col[4]);
          selection = 4;
        }
      }
      else if (selection == 10)
      {
        swap(col[selection], col[selection+1]);
                selection++;
      }
      aideOptionsJeu(aide, selection);

      break;
    case 'q':
    {
      return mainMenu(0);
    }
    case '\n':
      switch (selection)
      {
      case 11:
        return mainMenu(0);
        break;
      case 3:
        return optionsJeu();
      case 1:
        if (this->difficulte < 3)
        {
          this->difficulte++;
        }
        else
        {
          this->difficulte = 1;
        }
        plateau2.print(41, 12, "                      ", WBLACK);

        break;
      case 10:
      {
        int k = 1;
        for (int i = nbjoueurshumain; i < 6; i++)
        {
          nom[i] = "Bot " + myitoa(k);
          k++;
        }
        return;
      }
      case 4 ... 9:
      {
        if (selection == 4 + nbjoueurshumain && nbjoueurshumain < nbjoueurs)
        {
          nbjoueurshumain = min(6, nbjoueurshumain + 1);
        }
        choix[selection] = saisie(23, 14 + 2 * (selection - 4), (selection > 4 ? 25 : 18), plateau2);
        nom[selection - 4] = choix[selection];
        plateau2.print(9, 14 + 2 * (nbjoueurs - nbjoueurshumain), "     ");

        aide.setBordureFine();
        aideOptionsJeu(aide, selection);
        continue;
      }
      case 2:
        log = saisie(61, 14, 25, plateau2);
        break;
      }
    }
  }
}

string Menu::saisie(int x, int y, int longueur, Window &plateau)
{
  int ch;
  echo();
  curs_set(1);

  for (int i = 0; i < longueur; i++)
  {
    plateau.print(x + i, y, ' ', WBLACK);
  }
  string chaine;

  move(LINES / 2 - 16 + y + 1, COLS / 2 - 50 + x + 1);

  do
  {
    ch = getch();
    if (isalnum(ch) || ch == '.')
    {
      chaine += ch;
    }
    if (ch == KEY_BACKSPACE)
    {
      plateau.print(x, y, "                     ", WBLACK);
      move(LINES / 2 - 16 + y + 1, COLS / 2 - 50 + x + 1);
      chaine.clear();
    }
    if (ch == '\n' && chaine.length() == 0)
    {
      move(LINES / 2 - 16 + y + 1, COLS / 2 - 50 + x + 1);
      plateau.print(x, y + 2, "Au moins 1 caractère!");
    }
    else if (ch == '\n')
    {
      plateau.print(x, y + 2, "                     ", WBLACK);
      break;
    }
  } while (chaine.length() < 13);

  curs_set(0);
  noecho();
  plateau.print(x, y, "                     ", WBLACK);

  return chaine;
}

void aideOptionsJeu(Window &aide, int s)
{
  aide.clear();
  switch (s)
  {
  case 1:
    aide.print(1, 0, "La difficulté détermine la pertinence du choix des cibles de l'IA.");
    aide.print(1, 4, "Modifier");
    aide.print(33, 4, "Entrée");
    break;
  case 0:
    aide.print(1, 0, "Détermine la vitesse à laquelle l'IA  joue. \n Peut être modifié pendant le jeu.");
    aide.print(1, 4, "Augmenter / Diminuer");
    aide.print(32, 4, "Flèches");
    break;
  case 2:
    aide.print(1, 0, "Le nom du fichier qui enregistrera toutes les action du jeu.");
    aide.print(1, 4, "Modifier");
    aide.print(32, 4, "Entrée");
    break;
  case 3:
    aide.print(1, 0, "Liste des joueurs humain.\n\n Laissez vide pour une partie entre joueurs virtuels.\n Réinitialiser");
    aide.print(33, 4, "Entrée");

    break;
  case 4 ... 9:
    aide.print(1, 0, "Appuyez sur entrer pour ajouter un\n joueur humain ou modifier son nom.");
    aide.print(1, 4, "Ajouter / Modifier");
    aide.print(33, 4, "Entrée");
    break;
  case 10:
    aide.print(1, 0, "Jouer avec la configuration actuelle");
    break;
  case 11:
    aide.print(1, 0, "Retour au menu");
    break;
  }
}

void Menu::topscores(Window &top)
{
  top.setBordureFine();
  wattron(top.win, A_UNDERLINE);
  top.print(8, 0, "TOP SCORES", BLUEBLACK);
  wattroff(top.win, A_UNDERLINE);

  // top.print(10, 14, "Retour", colSelectionnee);

  int i = 3;
  string ligne;

  ifstream file("topscores", ios::in);

  while (getline(file, ligne) && i < 13)
  {
    top.print(1, i, ligne, BLUEBLACK);
    getline(file, ligne);
    int sc = atoi(ligne.c_str());
    string strsc = myitoa(sc / 1000) + " " + (sc % 1000 < 100 && sc % 1000 > 0 ? "0" : "") + myitoa(sc % 1000) + (sc % 1000 == 0 ? "00" : "");

    top.print(24 - strsc.length(), i, strsc, GBLACK);

    i++;
    usleep(100000);
  }
  file.close();
}

void Menu::PasDeSauvegarde()
{
  Window warning(6, 30, -15, -3);
  warning.setCouleurBordure(BRED);
  warning.setBordureFine();
  warning.print(0, 2, "Aucune sauvegarde disponible!", BLUEBLACK);
  usleep(1750000);
  refreshMenu();
}

void Menu::chargement()
{

  ifstream srcconfig("save/config.txt", ios::binary);
  if (!srcconfig)
  {
    PasDeSauvegarde();

    return;
  }
  ofstream destconfig("config/config.txt", ios::binary);
  destconfig << srcconfig.rdbuf();
  srcconfig.close();
  destconfig.close();

  string ligne;
  ifstream save("save/sauvegarde", ios::in);
  getline(save, this->log);
  getline(save, ligne);
  int humains = ligne[0] - '0';
  int nbjoueurs = ligne[2] - '0';
  int joueur = ligne[4] - '0';
  int JoueursRestants = ligne[6] - '0';
  // int cible = ligne[8] - '0';
  // int cibleSelectionnee = ligne[10] - '0';
  int nbhisto = ligne[12] - '0';
  int difficulte = ligne[14] - '0';
  getline(save, ligne);
  int tour = atoi(ligne.c_str());

  int ***EtatCases = new int **[nbjoueurs];
  int ***Case2 = new int **[nbjoueurs];

  for (int i = 0; i < nbjoueurs; i++)
  {
    EtatCases[i] = new int *[W];
    Case2[i] = new int *[W];
    for (int x = 0; x < W; x++)
    {
      EtatCases[i][x] = new int[H];
      Case2[i][x] = new int[H];
    }
  }

  string nom[6];
  int missilesTires[6];
  int missilesGagnants[6];

  int payback[6];
  getline(save, ligne);

  for (int i = 0; i < 6; i++)
  {
    payback[i] = ligne[i] - '0';
  }
  getline(save, ligne);

  int estVulnerable[6];
  for (int i = 0; i < 6; i++)
  {
    estVulnerable[i] = ligne[i] - '0';
  }
  getline(save, ligne);

  int positionDuJoueur[6];
  for (int i = 0; i < 6; i++)
  {
    positionDuJoueur[i] = ligne[i] - '0';
  }
  getline(save, ligne);

  int joueurEnPosition[6];
  for (int i = 0; i < 6; i++)
  {
    joueurEnPosition[i] = ligne[i] - '0';
  }
  getline(save, ligne);

  int *naviresRestants = new int[nbjoueurs];
  int k = 0;

  for (int i = 0; i < nbjoueurs; i++)
  {
    naviresRestants[i] = ligne[k] - '0';
    k += 2;
  }
  getline(save, ligne);
  k = 0;

  int **casesRestantes = new int *[nbjoueurs];
  string tmp;

  // Cases restantes de chacun des navires
  for (int i = 0; i < nbjoueurs; i++)
  {
    casesRestantes[i] = new int[5];
    for (int j = 0; j < 5; j++)
    {
      while (ligne[k] != ':')
      {
        tmp += ligne[k];
        k++;
      }
      casesRestantes[i][j] = atoi(tmp.c_str());
      k++;
      tmp.clear();
    }
    k = 0;
    getline(save, ligne);
  }

  int ***posNavires = new int **[nbjoueurs];

  // Position des navires
  for (int i = 0; i < nbjoueurs; i++)
  {
    k = 0;
    posNavires[i] = new int *[5];
    for (int j = 0; j < 5; j++)
    {
      posNavires[i][j] = new int[2];
      for (int p = 0; p < 2; p++)
      {
        while (ligne[k] != ':')
        {
          tmp += ligne[k];
          k++;
        }
        posNavires[i][j][p] = atoi(tmp.c_str());
        k++;
        tmp.clear();
      }
      k++;
    }

    getline(save, ligne);
  }

  int **NbPivotements = new int *[nbjoueurs];

  for (int i = 0; i < nbjoueurs; i++)
  {
    k = 0;
    NbPivotements[i] = new int[5];
    for (int n = 0; n < 5; n++)
    {
      NbPivotements[i][n] = ligne[k] - '0';
      k += 2;
    }
    getline(save, ligne);
  }

  k = 0;

  for (int i = 0; i < nbjoueurs; i++)
  {
    nom[i] += ligne;
    getline(save, ligne);
  }

  string historique[6];

  for (int i = 0; i < nbhisto; i++)
  {
    getline(save, ligne);

    while (ligne[1] != ':')
    {
      historique[i] += ligne;
      getline(save, ligne);
    }
  }

  getline(save, ligne);

  getline(save, ligne);

  int tailleFlotte = atoi(ligne.c_str());

  getline(save, ligne);

  for (int i = 0; i < nbjoueurs; i++)
  {
    getline(save, ligne);
    missilesTires[i] = atoi(ligne.c_str());
    getline(save, ligne);
    missilesGagnants[i] = atoi(ligne.c_str());
  }

  for (int n = 0; n < nbjoueurs; n++)
  {
    getline(save, ligne);

    k = 0;
    for (int y = 0; y < H; y++)
    {
      getline(save, ligne);
      k = 0;
      for (int x = 0; x < W; x++)
      {
        EtatCases[n][x][y] = ligne[k] - '0';
        k += 2;
      }
    }
    k = 0;
    getline(save, ligne);

    for (int y = 0; y < H; y++)
    {
      getline(save, ligne);
      k = 0;
      for (int x = 0; x < W; x++)
      {
        Case2[n][x][y] = ligne[k] - '0';
        k += 2;
      }
    }
  }

  save.close();

  string cheminlog = "save/" + this->log;
  ifstream src(cheminlog.c_str(), ios::binary);
  string logdest = "logs/" + this->log;
  ofstream dest(logdest.c_str(), ios::binary);
  dest << src.rdbuf();
  src.close();
  dest.close();

  bool stop = false;
  Window warning(6, 30, -15, -3);
  warning.setCouleurBordure(BRED);
  warning.setBordureFine();
  warning.print(0, 0, "Partie chargée.\nSouhaitez-vous supprimer\nles fichiers de sauvegarde?", BLUEBLACK);
  int ch;
  Color col[2] = {WBLUE, BBLUE};
  while (!stop)
  {
    ch = getch();
    warning.print(7, 4, "Oui", col[0]);
    warning.print(20, 4, "Non", col[1]);
    switch (ch)
    {
    case KEY_RIGHT:
      swap(col[0], col[1]);
      break;
    case KEY_LEFT:
      swap(col[0], col[1]);
      break;
    case '\n':
      stop = true;
      if (col[0] == WBLUE)
      {

        remove(cheminlog.c_str());
        remove("save/config.txt");
        remove("save/sauvegarde");
        warning.clear();
        warning.print(0, 2, "Suppression effectuée.");
        usleep(1500000);
      }
      break;
    }
  }

  clear();

  Jeu Jeu(nbjoueurs, humains, difficulte, 6, nom, log, true);
  Jeu.chargementparametres(joueur,JoueursRestants, tour, nbhisto, payback, estVulnerable, positionDuJoueur, joueurEnPosition, historique, EtatCases, Case2, casesRestantes, posNavires, naviresRestants, NbPivotements, missilesTires, missilesGagnants, tailleFlotte);
  Jeu.Phase2();

  for (int i = 0; i < nbjoueurs; i++)
  {
    for (int j = 0; j < W; j++)
    {
      delete[] EtatCases[i][j];
      delete[] Case2[i][j];
    }
    for (int j = 0; j < 5; j++)
    {
      delete[] posNavires[i][j];
    }
  
    delete[] NbPivotements[i];
    delete[] posNavires[i];
    delete[] EtatCases[i];
    delete[] Case2[i];
    delete[] casesRestantes[i];
  }

delete[] NbPivotements;
delete[] posNavires;
delete[] EtatCases;
delete[] Case2;
delete[] naviresRestants;
delete[] casesRestantes;
}

void ParametreLancement(char *argv)
{
  string par = argv;
  if (par == "version")
  {
    cout << version() << endl;
  }
  else if (par == "auteurs")
  {
    cout << aPropos() << endl;
    ;
  }
  else if (par == "aide")
  {
    cout << aide() << endl;
  }
  else
  {
    cout << "Commande inconnue.\nParamètres possibles : aide version auteurs" << endl;
  }
}

int main(int argc, char *argv[])
{

  if (argc > 1)
  {
    ParametreLancement(argv[1]);
  }
  else
  {
    startProgramX();
    if (COLS < 140 || LINES < 40)
    {
      erreurEcran();
    }
    Menu menu;

    menu.mainMenu(1);
  }

  return 0;
}
