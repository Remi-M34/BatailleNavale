#include "../include/headers.h"

using namespace std;

#define xm (COLS - 100) / 2
#define ym (LINES - 33) / 2

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
    case 38:
      colSelectionnee = convertColor(ligne);
      break;
    case 39:
      colNonSelectionnee = convertColor(ligne);
      break;
    case 40:
      if (ligne[0] == 'o' || ligne[0] == 'O')
      {
        bordureMenuDroite = true;
      }
      break;
    case 41:
      colBordureMenu = convertColor(ligne);
      break;
    case 42:
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

void Menu::mainMenu()
{

  startProgramX();

  Window plateau(33, 100, xm, ym, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }

  Window ap(2, 40, 50 - 40 / 2 + xm, 25 + ym, ' ');
  ap.setCouleurBordure(WBLACK);

  Color col[5] = {colSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee};

  refresh();
  getch();

  int c;
  string choix[5] = {"Commencer une partie", "Options", "Aide", "Version du jeu", "A Propos"};
  int selection = 0;

  Flotte flotte(50 - getDimFlotte('w') + xm, 4 + ym, 1);

  bool wait = false;

  while ((c = getch()) != 'q')
  {

    for (int i = 0; i < 5 && !wait; i++)
    {
      plateau.print(50 - choix[i].length() / 2, 12 + 2 * i, choix[i], col[i]);
    }
    switch (c)
    {
    case KEY_UP:
      if (selection != 0)
      {
        swap(col[selection], col[selection - 1]);
        selection--;
      }
      break;

    case KEY_DOWN:
      if (selection != 4)
      {
        swap(col[selection], col[selection + 1]);
        selection++;
      }

      break;
    case '\n':
      if (selection == 0)
      {
        erase();
        plateau.setCouleurBordure(BYELLOW);
        stopProgramX();
        return;
      }
      else if (selection == 1)
      {
        options();
        mainMenu();
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
        ap.clear();
        ap.print(0, 1, version());
        ap.setBordureDroite();
      }
      else if (selection == 4)
      {
        ap.clear();
        ap.print(0, 1, aPropos());
        ap.setBordureDroite();
      }
      else
      {
        changerDim();
        mainMenu();
        return;
      }
      break;
    }
  }

  erase();

  stopProgramX();
}

// Menu  des options

void Menu::options()
{
  // stopProgramX();
  // startProgramX();

  Window plateau(33, 100, xm, ym, carBordureMenu);
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

  Flotte flotte(50 - getDimFlotte('w') + xm, 4 + ym, 0);

  for (int i = 0; i < 5; i++)
  {
    plateau.print(50 - choix[i].length() / 2, 12 + 2 * i, choix[i], col[i]);
    usleep(12000);
  }

  while ((c = getch()) != 'q')
  {

    for (int i = 0; i < 5; i++)
    {
      plateau.print(50 - choix[i].length() / 2, 12 + 2 * i, choix[i], col[i]);
    }
    switch (c)
    {
    case KEY_UP:
      if (selection != 0)
      {
        swap(col[selection], col[selection - 1]);
        selection--;
      }
      break;

    case KEY_DOWN:
      if (selection != 4)
      {
        swap(col[selection], col[selection + 1]);
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
  Window aidef(19, 85, 7 + xm, 12 + ym);
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
  // stopProgramX();
  // startProgramX();

  int ch;
  int n = 0;

  Window plateau(33, 100, xm, ym, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }
  Window dim(3, 18, 41 + xm, 13 + ym, ' ');
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
  Color col[3] = {colSelectionnee, colNonSelectionnee, colNonSelectionnee};
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
            curs_set(0);
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
            curs_set(0);

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

void Menu::menuNavire()
{
  // stopProgramX();
  // startProgramX();

  int c;
  int n = 0;

  Window plateau(33, 100, xm, ym, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }
  Window aide(12, 30, 2 + xm, 11 + ym);
  aide.setBordureDroite();
  aide.print(1, 1, "<-, ->     Parcourir", WBLACK);
  aide.print(1, 2, "ENTREE     Sélectionner", WBLACK);
  aide.print(1, 11, "q          Menu", WBLACK);

  Color col = colNonSelectionnee;
  Flotte flotte(50 - getDimFlotte('w') + xm, 4 + ym, 0);

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

  Window plateau(33, 100, xm, ym, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }

  // Window aide(12, 30, 2 + xm, 11 + ym);
  // // aide.setBordureDroite();

  Color col[6] = {plateau.getCouleurFenetre(), colSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee, colNonSelectionnee};

  string th = "[ Thème actuel: " + theme + " ]";
  string choix[6] = {th, "Defaut", "Theme2", "Theme3", "Theme4", "Retour"};

  int selection = 1;

  Flotte flotte(50 - getDimFlotte('w') + xm, 4 + ym, 0);

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

  Window plateau(33, 100, xm, ym, carBordureMenu);
  plateau.setCouleurBordure(colBordureMenu);
  if (bordureMenuDroite)
  {
    plateau.setBordureDroite();
  }

  plateau.print(47, 22, "Valider", colSelectionnee);

  Window aide(5, 24, xm + 50 - 12, ym + 25);
  aide.setBordureDroite();
  aide.print(1, 1, "<-, ->       Parcourir", WBLACK);
  aide.print(1, 2, "ENTREE         Valider", WBLACK);
  aide.print(1, 4, "q                 Menu", WBLACK);

  Window info(6, 30, xm + 50 - 15, ym + 3);
  info.setBordureDroite();

  int selection = s;

  changePreset(selection);
  Flotte flotte(50 - getDimFlotte('w') + xm, 14 + ym, 0);

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
