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

using namespace std;

Menu::Menu()
{
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
    case 42:
      colSelectionnee = convertColor(ligne);
      break;
    case 43:
      colNonSelectionnee = convertColor(ligne);
      break;
    case 44:
      if (ligne[0] == 'o' || ligne[0] == 'O')
      {
        bordureMenuDroite = true;
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
      for (int i = 0; isalnum(ligne[i]); i++)
      {
        theme += ligne[i];
      }
      couleurs.close();
      return;
    }
    lignes++;
  }

  couleurs.close();
}

void Menu::mainMenu(int d)
{

  // startProgramX();

  Window plateau(33, 100, -50, -16, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }

  Window apropos(2, 40, 50 - 40 / 2 + -50, 25 + -16, ' ');
  apropos.setCouleurBordure(WBLACK);

  Color col[5] = {colSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee};

  refresh();
  getch();

  int c;
  string choix[5] = {"Commencer une partie", "Options", "Aide", "Version du jeu", "A Propos"};
  int selection = 0;

  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, d);
  while ((c = getch()))
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
        checkTailleEcran();
        selectJoueurs(plateau);
        optionsJeu();
        // if (nbjoueurs >= 2)

        // erase();
        Jeu Jeu(nbjoueurs, nbjoueurshumain, difficulte, vitesse, nom);
        Jeu.start();
        return;
      }
      else if (selection == 1)
      {
        options();
        mainMenu(1);
        return;
        // version.print(2, 2, "aaaaaaaaaaaaaaa", WGREEN);
        // refresh();
      }
      else if (selection == 2)
      {
        aideMenu();
      }
      else if (selection == 3)
      {
        apropos.clear();
        apropos.print(0, 1, version());
        apropos.setBordureDroite();
      }
      else if (selection == 4)
      {
        apropos.clear();
        apropos.print(0, 1, aPropos());
        apropos.setBordureDroite();
      }
      else
      {
        changerDim();
        mainMenu(1);
        return;
      }
      break;
    }
  }

  erase();

  stopProgramX();
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
  // stopProgramX();
  // startProgramX();

  Window plateau(33, 100, -50, -16, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }

  Color col[5] = {colSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee};

  refresh();
  getch();

  int c;
  string choix[5] = {"Editeur de navires", "Modifier la taille de la grille", "Thèmes", "Charger flotte", "Retour"};
  int selection = 0;

  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0);

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
        options();
        return;
      }
      else if (selection == 1)
      {
        changerDim();

        options();
        return;
        // version.print(2, 2, "aaaaaaaaaaaaaaa", WGREEN);
        // refresh();
      }
      else if (selection == 2)
      {
        themes();
        options();
        return;
      }
      else if (selection == 3)
      {
        preset(1);
        options();
        return;
      }
      else if (selection == 4)
      {
        return;
        // version.print(2, 2, "aaaaaaaaaaaaaaa", WGREEN);
        // refresh();
      }
      break;
    }
  }

  erase();

  // stopProgramX();
}

void Menu::aideMenu()
{
  int ch;
  Window aidef(19, 85, 7 + -50, 12 + -16);
  aidef.setBordureDroite();
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

  Window plateau(33, 100, -50, -16, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }

  Window dim(3, 18, 41 + -50, 13 + -16, ' ');
  dim.setCouleurBordure(BGREEN);
  dim.setCouleurFenetre(WBLACK);
  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0);

  Window aide(8, 30, 2 + -50, 11 + -16);
  aide.setBordureDroite();
  aide.print(1, 1, "<-, ->     Parcourir", WBLACK);
  aide.print(1, 2, "ENTREE     Sélectionner", WBLACK);

  aide.print(1, 7, "q          Menu", WBLACK);

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
        for (int i = 0; i < width.length(); i++)
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
        for (int i = 0; i < height.length(); i++)
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
  // stopProgramX();
  // startProgramX();

  int c;
  int n = 0;

  Window plateau(33, 100, -50, -16, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }
  Window aide(12, 30, 2 + -50, 11 + -16);
  aide.setBordureDroite();
  aide.print(1, 1, "<-, ->     Parcourir", WBLACK);
  aide.print(1, 2, "ENTREE     Sélectionner", WBLACK);
  aide.print(1, 11, "q          Menu", WBLACK);

  Color col = colNonSelectionnee;
  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0);

  flotte.initSelection();

  while ((c = getch()) != 'q')
  {
    plateau.print(15, 5, "Retour", col);

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
  int n = 0;

  Window plateau(33, 100, -50, -16, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }

  // Window aide(12, 30, 2 + -50, 11 + -16);
  // // aide.setBordureDroite();

  Color col[6] = {plateau.getCouleurFenetre(), colSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee};

  string th = "[ Thème actuel: " + theme + " ]";
  string choix[6] = {th, "Defaut", "Theme2", "Theme3", "Theme4", "Retour"};

  int selection = 1;

  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0);

  while ((c = getch()) != 'q')
  {

    for (int i = 0; i < 6; i++)
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
      if (selection != 5)
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
        theme = "Theme3";
        break;
      case 4:
        theme = "Theme4";
        break;
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

  Window plateau(33, 100, -50, -16, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }

  plateau.print(47, 22, "Valider", colSelectionnee);

  Window aide(5, 24, -50 + 50 - 12, -16 + 25);
  aide.setBordureDroite();
  aide.print(1, 1, "<-, ->       Parcourir", WBLACK);
  aide.print(1, 2, "ENTREE         Valider", WBLACK);
  aide.print(1, 4, "q                 Menu", WBLACK);

  Window info(6, 30, -50 + 50 - 15, -16 + 3);
  info.setBordureDroite();

  int selection = s;

  changePreset(selection);
  Flotte flotte(50 - getDimFlotte('w') + -50, 14 + -16, 0);

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
    Window erreur(5, 100, -50, -16, '+');
    erreur.print(0, 0, erreurTailleFenetre2());
    cin.ignore(1);
    assert(COLS > 5 * getWidthGrille() && LINES > 2.5 * getHeightGrille());
  }
}

void Menu::optionsJeu()
{
  for (int i = 0 ; i < 6 ; i ++)
  {
    nom[i] = "Bot "+myitoa(i);
  }

  int c;
  nbjoueurshumain = 0;
  Window plateau(33, 100, -50, -16, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }
  Window aide(6, 40, -50 + 30, 26 + -16, ' ');
  aide.setBordureDroite();

  Color col[10] = {
      colNonSelectionnee,
      colNonSelectionnee,
      colSelectionnee,
      colNonSelectionnee,
      colNonSelectionnee,
      colNonSelectionnee,
      colNonSelectionnee,
      colNonSelectionnee,
      colNonSelectionnee,
      colNonSelectionnee,
  };

  string choix[10] = {"Vitesse IA: x", "Difficulté: ", "Humains", "Ajouter un joueur", "Ajouter un joueur humain",
                      "Ajouter un joueur humain", "Ajouter un joueur humain",
                      "Ajouter un joueur humain", "Ajouter un joueur humain", "Jouer"};
  string difficulte[3] = {"Facile", "Normal", "Difficile"};
  int selection = 2;

  Flotte flotte(50 - getDimFlotte('w') + -50, 4 + -16, 0);
  aideOptionsJeu(aide, selection);

  while ((c = getch()) != 'q')
  {

    plateau.print(70, 12, choix[0] + myitoa(vitesse), col[0]);
    plateau.print(41, 12, choix[1] + difficulte[this->difficulte - 1], col[1]);
    plateau.print(23, 12, choix[2] + " (" + myitoa(nbjoueurshumain) + ")", col[2]);
    plateau.print(9, 12, "IA (" + myitoa(nbjoueurs - nbjoueurshumain) + ")", colNonSelectionnee);
    plateau.print(48, 24, choix[9], col[9]);

    for (int i = 3; i < nbjoueurshumain + 4 && i < nbjoueurs + 3; i++)
    {

      plateau.print(23, 14 + 2 * (i - 3), choix[i], col[i]);
    }
    switch (c)
    {
    case KEY_UP:
      if (selection > 2 && selection < 9)
      {
        swap(col[selection], col[selection - 1]);
        selection--;
      }
      else if (selection == 0)
      {
        vitesse = min(9, vitesse + 1);
      }
      else if (selection == 9)
      {
        swap(col[selection], col[1]);
        selection = 1;
      }
      aideOptionsJeu(aide, selection);

      break;

    case KEY_DOWN:
      if (selection < nbjoueurshumain + 3 && selection > 1 && selection < 2 + nbjoueurs)
      {
        swap(col[selection], col[selection + 1]);
        selection++;
      }
      else if (selection == 0)
      {
        vitesse = max(0, vitesse - 1);
      }
      else if (selection == 1 || selection == min(nbjoueurs, nbjoueurshumain) + 3 || selection == 2 + nbjoueurs)
      {
        swap(col[selection], col[9]);
        selection = 9;
      }
      aideOptionsJeu(aide, selection);

      break;
    case KEY_RIGHT:
      if (selection > 0 && selection < 3)
      {
        swap(col[selection], col[selection - 1]);
        selection--;
      }
      aideOptionsJeu(aide, selection);

      break;

    case KEY_LEFT:
      if (selection < 2 && selection >= 0)
      {
        swap(col[selection], col[selection + 1]);
        selection++;
      }
      aideOptionsJeu(aide, selection);

      break;
    case '\n':
      switch (selection)
      {
      case 0:

        break;
      case 1:
        if (this->difficulte < 3)
        {
          this->difficulte++;
        }
        else
        {
          this->difficulte = 1;
        }
        plateau.print(41, 12, "                      ", WBLACK);

        break;
      case 9:

        return;

      case 3 ... 8:
      {
        if (selection == 3 + nbjoueurshumain && nbjoueurshumain < nbjoueurs)
        {
          nbjoueurshumain = min(6, nbjoueurshumain + 1);
        }
        choix[selection] = saisie(23, 14 + 2 * (selection - 3), 25, plateau);
        nom[selection-3] = choix[selection];
        continue;
      }
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
    if (isalnum(ch))
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
  } while (chaine.length() < 10);

  curs_set(0);
  noecho();

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
    aide.print(1, 0, "Liste des joueurs humain.\n\n Laissez vide pour une partie entre joueurs virtuels.");

  case 3 ... 8:
    aide.print(1, 0, "Appuyez sur entrer pour ajouter un\n joueur humain ou modifier son nom.");
    aide.print(1, 4, "Ajouter / Modifier");
    aide.print(33, 4, "Entrée");
    break;
  case 9:
    aide.print(1, 0, "Jouer avec la configuration actuelle");
    break;
  }
}