#include "../include/grille.h"
#include "../include/window.h"
#include "../include/scores.h"

#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;
#define H getHeightGrille()
#define W getWidthGrille()

Grille::Grille(int const sx, int const sy, int sxf, int syf, int &vitesse, int const difficulte, Window &aide, bool estIA, string nom, bool partiechargee) : fenetre(H, W * 2, sx, sy, ' '), flotte(sxf, syf, 1, partiechargee), IA(estIA),

                                                                                                                                                             vitesse(vitesse), aide(&aide), difficulte(difficulte), Score(difficulte), nom(nom)

{

      navire = listedesnavires();
      joue = false;
      naviresRestants = 5;
      focusnavire = -1;
      focusx = -1;
      focusy = -1;
      retourmenu = false;
      retoursauvegarde = false;
      carGrilleMauvaiseSelection = '!';
      for (int i = 0; i < 5; i++)
      {
            caseRestantes[i] = 0;
            NombrePivotements[i] = 0;
            if (i < 2)
            {
                  dernierePositionGagnante[i] = -1;
                  dernierTir[i] = -1;
            }
      }
      //Initialise la grille et rend toutes les cases de la grille VIDE
      initCouleurs();
      init();
}

Grille::~Grille()
{

      // delete[] Case;
      // delete[] Case2;
}

void Grille::selectionNavire()
{
      // Ré-initialisation des couleurs...
      initCouleurs();
      mvwprintw(fenetre.frame, H + 1, W - nom.length() / 2, nom.c_str());
      wrefresh(fenetre.frame);
      //Si le joueur est une IA
      if (IA)
      {
            placementAleatoire();
            return;
      }
      else
      {
            flotte.initSelection();
      }

      int ch;
      int n = flotte.getPremierNavire();

      cin.seekg(0, ios::end);
      cin.clear();

      while ((ch = getch()) != 'q')

      {
            switch (ch)
            {
            case KEY_RIGHT:
                  if (n != flotte.getDernierNavire())
                  {
                        flotte.echangeSelection(n, flotte.getNavireSuivant(n));
                        n = flotte.getNavireSuivant(n);
                        flotte.refreshPort(0);
                  }

                  break;

            case KEY_LEFT:
                  if (n != flotte.getPremierNavire())
                  {
                        flotte.echangeSelection(n, flotte.getNavirePrecedent(n));
                        n = flotte.getNavirePrecedent(n);
                        flotte.refreshPort(0);
                  }
                  break;
            case 'a':
            {
                  placementAleatoire();
                  fenetre.clear();
                  usleep(999999);
                  return;
            }

            case '\n':
                  flotte.estAuPort(n, false);
                  placementNavire(n);

                  if (flotte.getEstAuPort(n))
                  {
                        break;
                  }

                  n = flotte.getPremierNavire();

                  if (n == -1)
                  {
                        colNavires = colVide;
                        refreshGrille(0, 0, W, H);
                        return;
                  }
                  else
                  {
                        flotte.initSelection();
                        flotte.refreshPort(0);
                  }
                  break;
            }
      }
}

void Grille::init()
{
      int const h = H;
      int const w = W;
      Case = new etat *[w];
      Case2 = new int *[w];

      for (int i = 0; i < w; i++)
      {
            Case[i] = new etat[h];
            Case2[i] = new int[h];

            for (int j = 0; j < h; j++)
            {
                  Case[i][j] = VIDE;
                  Case2[i][j] = 9;
                  fenetre.print(2 * i, j, ' ');
                  fenetre.print(2 * i + 1, j, ' ');
            }
      }
      mvwprintw(fenetre.frame, H + 1, W - nom.length() / 2, nom.c_str());
      wrefresh(fenetre.frame);
}

void Grille::refreshGrille(int fx, int fy, int maxX, int maxY)
{

      if (maxX == 0 && maxY == 0)
      {
            maxX = min(W, fx + 7);
            maxY = min(H, fy + 7);
      }
      if (fx < 0)
      {
            fx = 0;
      }
      if (fy < 0)
      {
            fy = 0;
      }

      for (int x = fx; x < maxX; x++)
      {
            for (int y = fy; y < maxY; y++)
            {
                  switch (Case[x][y])
                  {
                  case VIDE:
                        fenetre.print(2 * x, y, ' ', colVide);
                        fenetre.print(2 * x + 1, y, ' ', colVide);
                        break;
                  case TOMBEALEAU:
                        fenetre.print(2 * x, y, ' ', colManque);
                        fenetre.print(2 * x + 1, y, ' ', colManque);
                        break;
                  case NAVIRE:
                        fenetre.print(2 * x, y, ' ', colVide);
                        fenetre.print(2 * x + 1, y, ' ', colVide);
                        break;
                  case TOUCHE:
                        fenetre.print(2 * x, y, '#', colTouche);
                        fenetre.print(2 * x + 1, y, '#', colTouche);
                        break;
                  case COULE:
                        fenetre.print(2 * x, y, '#', colCoule);
                        fenetre.print(2 * x + 1, y, '#', colCoule);
                        break;
                  }
            }
      }
}

void Grille::refreshNavireGrille(int n, int fx, int fy)
{

      if (fx < 0)
      {
            fx = 0;
      }
      if (fy < 0)
      {
            fy = 0;
      }

      int maxX = min(W, fx + 2 + flotte.getWidthnavire(n));
      int maxY = min(H, fy + 2 + flotte.getHeightnavire(n));

      for (int x = fx; x < maxX; x++)
      {
            for (int y = fy; y < maxY; y++)
            {
                  switch (Case[x][y])
                  {
                  case VIDE:
                        fenetre.print(2 * x, y, ' ', colVide);
                        fenetre.print(2 * x + 1, y, ' ', colVide);
                        break;
                  case TOMBEALEAU:
                        fenetre.print(2 * x, y, ' ', colManque);
                        fenetre.print(2 * x + 1, y, ' ', colManque);

                        break;
                  case NAVIRE:
                        fenetre.print(2 * x, y, ' ', colNavires);
                        fenetre.print(2 * x + 1, y, ' ', colNavires);
                        break;
                  case TOUCHE:
                        fenetre.print(2 * x, y, '#', colTouche);
                        fenetre.print(2 * x + 1, y, '#', colTouche);
                        break;
                  case COULE:
                        fenetre.print(2 * x, y, '#', colCoule);
                        fenetre.print(2 * x + 1, y, '#', colCoule);
                        break;
                  }
            }
      }
}

void Grille::placementNavire(int n)
{

      // refreshGrille(0, 0, 0, 0);
      int fx = (W - flotte.getWidthnavire(n)) / 2;
      int fy = (H - flotte.getHeightnavire(n)) / 2;
      int ch;
      int x = 0;
      int y = 0;

      char c = ' ';
      if (!(checkPlacement(n, fx, fy)))
      {
            c = '#';
      }

      for (x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (y = 0; y < flotte.getHeightnavire(n); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        fenetre.print(2 * (x + fx), y + fy, c, colNavires);
                        fenetre.print(2 * (x + fx) + 1, y + fy, c, colNavires);
                  }
            }
      }
      x = 0;
      y = 0;
      x = (W - flotte.getWidthnavire(n)) / 2;
      y = (H - flotte.getHeightnavire(n)) / 2;

      while ((ch = getch()) != 'q')
      {
            switch (ch)
            {
            case KEY_RIGHT:
                  if (x < W - flotte.getWidthnavire(n))
                  {
                        x++;

                        moveNavire(n, x, y);
                  }

                  break;
            case KEY_LEFT:
                  if (x > 0)
                  {
                        x--;

                        moveNavire(n, x, y);
                  }

                  break;
            case KEY_UP:
                  if (y > 0)
                  {
                        y--;
                        moveNavire(n, x, y);
                  }
                  break;
            case KEY_DOWN:

                  if (y < H - flotte.getHeightnavire(n))
                  {
                        y++;
                        moveNavire(n, x, y);
                  }
                  break;

            case ' ':
                  refreshNavireGrille(n, x, y);
                  pivoteDroite(n, x, y);
                  moveNavire(n, x, y);
                  break;

            case 'w':
            {
                  refreshNavireGrille(n, x, y);
                  pivoteDroite(n, x, y);
                  pivoteDroite(n, x, y);
                  pivoteDroite(n, x, y);
                  moveNavire(n, x, y);
                  break;
            }
            case '\n':

                  if (checkPlacement(n, x, y))
                  {
                        validerNavire(n, x, y);

                        // refreshGrille(0, 0, 0, 0);
                        return;
                  }
                  else
                  {
                        fenetre.setCouleurBordure(colMauvaiseCouleur);
                        mvwprintw(fenetre.frame, H + 1, W - nom.length() / 2, nom.c_str());
                        wrefresh(fenetre.frame);
                  }

                  break;
            case 'b':
                  flotte.estAuPort(n, true);
                  refreshGrille(x, y, 0, 0);
                  return;
                  break;
            }
      }
}
void Grille::moveNavire(int n, int sx, int sy)
{
      if (fenetre.getCouleurBordure() != bordure)
      {
            fenetre.setCouleurBordure(bordure);
            fenetre.setCarBordure(carBordureGrille);
            mvwprintw(fenetre.frame, H + 1, W - nom.length() / 2, nom.c_str());
            wrefresh(fenetre.frame);
      }
      int x = 0;
      int y = 0;
      char c = ' ';
      char c2 = ' ';
      if (!checkPlacement(n, sx, sy))
      {
            c = '#';
      }
      refreshNavireGrille(n, sx - 1, sy - 1);

      for (x = 0; x < fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)); x++)
      {
            for (y = 0; y < fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n)); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        fenetre.print(2 * (sx) + 2 * x, sy + y, c, colNavires);
                        fenetre.print(2 * (sx) + 1 + 2 * x, sy + y, c2, colNavires);
                  }
            }
      }
}

void Grille::pivoteDroite(int n, int &xx, int &y)
{
      int taille = fmax(flotte.getHeightnavire(n), flotte.getWidthnavire(n));

      for (int x = 0; x < taille / 2; x++)
      {

            for (int y = x; y < taille - x - 1; y++)
            {
                  int temp = navire[n][x][y];

                  navire[n][x][y] = navire[n][y][taille - 1 - x];

                  navire[n][y][taille - 1 - x] = navire[n][taille - 1 - x][taille - 1 - y];

                  navire[n][taille - 1 - x][taille - 1 - y] = navire[n][taille - 1 - y][x];

                  navire[n][taille - 1 - y][x] = temp;
            }
      }

      checkRepositionnement(n);
      flotte.swapDimensionsNavire(n);
      if (xx + flotte.getWidthnavire(n) > W)
      {
            xx -= abs(flotte.getHeightnavire(n) - flotte.getWidthnavire(n));
      }
      if (y + flotte.getHeightnavire(n) > H)
      {
            y -= abs(flotte.getHeightnavire(n) - flotte.getWidthnavire(n));
      }
      if (NombrePivotements[n] == 3)
      {
            NombrePivotements[n] = 0;
      }
      else
      {
            NombrePivotements[n]++;
      }
}

void Grille::checkRepositionnement(int n)
{

      if (flotte.getHeightnavire(n) != flotte.getWidthnavire(n))
      {
            int k = 0;
            for (int x = 0; navire[n][x][0] != -1; x++)
            {
                  k += navire[n][x][0];
            }
            if (k == 0)
            {
                  repositionnementVertical(n);
                  checkRepositionnement(n);
            }
            k = 0;
            for (int y = 0; navire[n][0][y] != -1; y++)
            {
                  k += navire[n][0][y];
            }
            if (k == 0)
            {
                  repositionnementHorizontal(n);
                  checkRepositionnement(n);
            }
      }
}

void Grille::repositionnementHorizontal(int n)
{
      int x = 0;
      for (x = 0; navire[n][x + 1][0] != -1; x++)
      {
            for (int y = 0; navire[n][0][y] != -1; y++)
            {
                  navire[n][x][y] = navire[n][x + 1][y];
            }
      }
      for (int y = 0; navire[n][0][y] != -1; y++)
      {
            navire[n][x][y] = 0;
      }
}

void Grille::repositionnementVertical(int n)
{
      int y = 0;
      for (int x = 0; navire[n][x][0] != -1; x++)
      {
            for (y = 0; navire[n][0][y + 1] != -1; y++)
            {
                  navire[n][x][y] = navire[n][x][y + 1];
            }
      }
      for (int x = 0; navire[n][x][0] != -1; x++)
      {
            navire[n][x][y] = 0;
      }
}

bool Grille::checkPlacement(int n, int sx, int sy)
{
      for (int x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (int y = 0; y < flotte.getHeightnavire(n); y++)
            {

                  if (Case[sx + x][sy + y] == NAVIRE && navire[n][x][y] == 1)
                  {
                        return false;
                  }
            }
      }
      return true;
}

void Grille::validerNavire(int n, int sx, int sy)
{
      posNavire[n][0] = sx;
      posNavire[n][1] = sy;

      for (int x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (int y = 0; y < flotte.getHeightnavire(n); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        Case[sx + x][sy + y] = NAVIRE;
                        Case2[sx + x][sy + y] = n;
                        caseRestantes[n]++;
                        Score.augmenterTailleFlotte();
                  }
            }
      }

      if (!estIA())
      {
            usleep(200000);
      }
}

void Grille::changerEtat(int x, int y, etat e)
{
      Case[x][y] = e;
}

void Grille::coulerNavire(int n)
{
      for (int x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (int y = 0; y < flotte.getHeightnavire(n); y++)
            {
                  if (navire[n][x][y] == 1)
                  {
                        Case[posNavire[n][0] + x][posNavire[n][1] + y] = COULE;
                  }
            }
      }
      refreshGrille(0, 0, 0, 0);
}

int Grille::destinationMissile()
{

      int ch;
      int x = W / 2;
      int y = H / 2;

      findMilieu(x, y);
      if (dernierePositionGagnante[0] == -1)
      {
            fenetre.print(2 * x, y, ' ', colCaseSelectionnee);
            fenetre.print(2 * x + 1, y, ' ', colCaseSelectionnee);
      }
      else
      {
            fenetre.print(2 * x, y, '#', colCaseSelectionnee);
            fenetre.print(2 * x + 1, y, '#', colCaseSelectionnee);
      }

      while ((ch = getch()))
      {
            refresh();
            wrefresh(fenetre.getWin());

            switch (ch)
            {

            case KEY_RIGHT:
                  moveRight(x, y);
                  break;
            case KEY_LEFT:
                  moveLeft(x, y);
                  break;

            case KEY_DOWN:
                  moveDown(x, y);
                  break;

            case KEY_UP:
                  moveUp(x, y);
                  break;
            case 'a':
                  refreshGrille(x, y, x + 1, y + 1);
                  return destinationMissileAleatoire();
            case 'e':
                  caseSuivante(x, y);
                  break;
            case 'z':
                  casePrecedente(x, y);
                  break;
            case 'c':
                  refreshGrille(x, y, x + 1, y + 1);
                  return -1;
                  break;
            case '+':
                  vitesse = min(vitesse + 1, 9);
                  (*aide).print((*aide).getWindowWidth() - 2, 0, myitoa(vitesse), BCYAN);
                  break;
            case '-':
                  vitesse = max(vitesse - 1, 0);
                  (*aide).print((*aide).getWindowWidth() - 2, 0, myitoa(vitesse), BCYAN);
                  break;
            case 's':
                  refreshGrille(x, y, x + 1, y + 1);
                  return -2;
                  break;
            case 'm':
                  return -3;
                  break;
            case 'n':
                  return -4;
                  break;
            case '\n':
                  dernierTir[0] = x;
                  dernierTir[1] = y;
                  if (Case[x][y] == VIDE)
                  {
                        Case[x][y] = TOMBEALEAU;
                        refreshGrille(x, y, x + 1, y + 1);
                        dernierePositionGagnante[0] = -1;
                        dernierePositionGagnante[1] = -1;
                        return 0;
                  }
                  else if (Case[x][y] == NAVIRE)
                  {
                        Case[x][y] = TOUCHE;
                        caseRestantes[Case2[x][y]]--;
                        if (estCoule(Case2[x][y]) == true)
                        {
                              refreshGrille(posNavire[Case2[x][y]][0], posNavire[Case2[x][y]][1], W, H);
                              dernierePositionGagnante[0] = x;
                              dernierePositionGagnante[1] = y;
                              focusnavire = findFocus();
                              naviresRestants--;
                              if (estMort())
                              {
                                    return 3;
                              }
                              else
                              {
                                    return 2;
                              }
                        }
                        else
                        {
                              refreshGrille(x, y, x + 1, y + 1);
                              dernierePositionGagnante[0] = x;
                              dernierePositionGagnante[1] = y;
                              focusnavire = Case2[x][y];
                              focusx = x;
                              focusy = y;
                              return 1;
                        }
                  }
                  break;
            }
      }

      return 0;
}

void Grille::caseSuivante(int &x, int &y)
{
      checkSpeedG();

      refreshGrille(x, y, x + 1, y + 1);

      do
      {
            if (x < W - 1)
            {
                  x++;
            }
            else
            {
                  if (y < H - 1)
                  {
                        y++;
                  }
                  else
                  {
                        y = 0;
                  }
                  x = 0;
            }
      } while (!caseNonDecouverte(x, y));

      fenetre.print(2 * (x), y, ' ', colCaseSelectionnee);
      fenetre.print(2 * x + 1, y, ' ', colCaseSelectionnee);

      fenetre.BordureNumerotee(x, y);
      return;
}

void Grille::casePrecedente(int &x, int &y)
{

      checkSpeedG();
      refreshGrille(x, y, x + 1, y + 1);

      do
      {
            if (x > 0)
            {
                  x--;
            }
            else
            {
                  if (y > 0)
                  {
                        y--;
                  }
                  else
                  {
                        y = H - 1;
                  }
                  x = W - 1;
            }
      } while (!caseNonDecouverte(x, y));

      fenetre.print(2 * (x), y, ' ', colCaseSelectionnee);
      fenetre.print(2 * x + 1, y, ' ', colCaseSelectionnee);
      fenetre.BordureNumerotee(x, y);
      return;
}

void Grille::moveRight(int &x, int &y)
{

      checkSpeedG();

      for (int i = x + 1; i < W; i++)
      {
            if (Case[i][y] == VIDE || Case[i][y] == NAVIRE)
            {
                  refreshGrille(x, y, W, y + 1);
                  fenetre.print(2 * i, y, ' ', colCaseSelectionnee);
                  fenetre.print(2 * i + 1, y, ' ', colCaseSelectionnee);
                  x = i;
                  fenetre.BordureNumerotee(x, y);

                  return;
            }
      }
}

void Grille::moveLeft(int &x, int &y)
{

      checkSpeedG();

      for (int i = x - 1; i >= 0; i--)
      {
            if (Case[i][y] == VIDE || Case[i][y] == NAVIRE)
            {
                  refreshGrille(0, y, x + 1, y + 1);
                  fenetre.print(2 * (i), y, ' ', colCaseSelectionnee);
                  fenetre.print(2 * i + 1, y, ' ', colCaseSelectionnee);
                  x = i;
                  fenetre.BordureNumerotee(x, y);

                  return;
            }
      }
}

void Grille::moveUp(int &x, int &y)
{

      checkSpeedG();

      for (int i = y - 1; i >= 0; i--)
      {
            if (Case[x][i] == VIDE || Case[x][i] == NAVIRE)
            {
                  refreshGrille(x, 0, x + 1, y + 1);
                  fenetre.print(2 * (x), i, ' ', colCaseSelectionnee);
                  fenetre.print(2 * x + 1, i, ' ', colCaseSelectionnee);
                  y = i;
                  fenetre.BordureNumerotee(x, y);

                  return;
            }
      }
}

void Grille::moveDown(int &x, int &y)
{

      checkSpeedG();

      for (int i = y + 1; i < H; i++)
      {
            if (Case[x][i] == VIDE || Case[x][i] == NAVIRE)
            {
                  refreshGrille(x, y, x + 1, H);
                  fenetre.print(2 * (x), i, ' ', colCaseSelectionnee);
                  fenetre.print(2 * x + 1, i, ' ', colCaseSelectionnee);
                  y = i;
                  fenetre.BordureNumerotee(x, y);

                  return;
            }
      }
}

void Grille::findMilieu(int &x, int &y)
{
      if (dernierePositionGagnante[0] != -1)
      {
            x = dernierePositionGagnante[0];
            y = dernierePositionGagnante[1];
            fenetre.BordureNumerotee(x, y);
            return;
      }

      int k = 1;
      while (Case[x][y] != VIDE || Case[x][y] != NAVIRE)
      {

            x = max(0, x - 1);
            y = max(0, y - 1);

            for (int i = x; (i <= x + 2 * k) && i < W; i++)
            {
                  for (int j = y; (j <= y + 2 * k) && (j < H); j++)
                  {
                        if (Case[i][j] == VIDE || Case[i][j] == NAVIRE)
                        {
                              x = i;
                              y = j;
                              fenetre.BordureNumerotee(x, y);

                              return;
                        }
                  }
            }
            k++;
      }
}

bool Grille::estCoule(int n)
{
      if (caseRestantes[n] == 0)
      {
            coulerNavire(n);
            return true;
      }
      else
      {
            return false;
      }
}

void Grille::initCouleurs()
{
      ifstream couleurs("config/couleurs.txt", ios::in);
      int lignes = 1;
      string ligne;

      while (getline(couleurs, ligne))
      {
            switch (lignes)
            {
            case 8:
                  colNavires = convertColor(ligne);
                  break;
            case 16:
                  fenetre.setCouleurBordure(convertColor(ligne));
                  bordure = convertColor(ligne);
                  break;
            case 17:
                  colMauvaiseCouleur = convertColor(ligne);
                  break;
            case 20:
                  carBordureGrille = ligne[0];
                  fenetre.setCarBordure(carBordureGrille);
                  break;

            case 26:
                  int i;
                  for (i = 0; isdigit(ligne[i]); i++)
                  {
                  }
                  ligne.erase(i, 150);
                  std::stringstream(ligne) >> delaiTirIA;
                  delaiTirIA *= 1000;
                  break;
            case 27:
                  for (i = 0; isdigit(ligne[i]); i++)
                  {
                  }
                  ligne.erase(i, 150);
                  std::stringstream(ligne) >> delaiPoseIA;
                  delaiPoseIA *= 1000;
                  break;
            case 28:
            case 33:
                  colCaseSelectionnee = convertColor(ligne);
                  break;
            case 34:
                  colTouche = convertColor(ligne);
                  break;
            case 35:
                  colCoule = convertColor(ligne);
                  break;
            case 36:
                  colManque = convertColor(ligne);
                  break;
            case 37:
                  colVide = convertColor(ligne);
                  couleurs.close();
                  return;
            }

            lignes++;
      }

      couleurs.close();
}

void Grille::placementAleatoire()
{
      int n = flotte.getRandomNavire();
      usleep(delaiPoseIA / 2);

      int x, y, p;
      srand((int)time(0));

      while (n != -1)
      {
            p = rand() % 3;

            for (int i = 0; i < p; i++)
            {
                  pivoteDroite(n, x, y);
            }
            x = rand() % (W - flotte.getWidthnavire(n));
            y = rand() % (H - flotte.getHeightnavire(n));

            if (checkPlacement(n, x, y))
            {
                  flotte.estAuPort(n, false);
                  validerNavire(n, x, y);
                  flotte.selectionne(n, false);

                  n = flotte.getRandomNavire();
                  usleep(delaiPoseIA);
            }

            if (n == -1)
            {
                  colNavires = colVide;
                  usleep(30000);

                  return;
            }
      }
}

int Grille::destinationMissileAleatoire()
{
      int x = focusx;
      int y = focusy;
      if (focusnavire == -1)
      {
            focusnavire = findFocus();
      }
      if (focusnavire == -1)
      {
            caseAleatoire(x, y);
      }
      else
      {
            zoneFocus(x, y);
            if (focusnavire == -1)
            {
                  return destinationMissileAleatoire();
            }
      }

      if (retourmenu)
      {
            return -3;
      }
      else if (retoursauvegarde)
      {
            retoursauvegarde = false;
            refreshGrille(0, 0, W, 3);
            return -2;
      }

      if (1)
      {
            int dx = W / 2, dy = H / 2;
            findMilieu(dx, dy);
            if (dernierePositionGagnante[0] == -1)
            {
                  fenetre.print(2 * dx, dy, ' ', colCaseSelectionnee);
                  fenetre.print(2 * dx + 1, dy, ' ', colCaseSelectionnee);
            }
            else
            {
                  fenetre.print(2 * dx, dy, '#', colCaseSelectionnee);
                  fenetre.print(2 * dx + 1, dy, '#', colCaseSelectionnee);
            }
            wait();
            deplacementIA(dx, dy, x, y);
      }
      else
      {
            fenetre.print(2 * x, y, ' ', colCaseSelectionnee);
            fenetre.print(2 * x + 1, y, ' ', colCaseSelectionnee);
      }
      wait();
      wait();
      wait();
      dernierTir[0] = x;
      dernierTir[1] = y;
      if (Case[x][y] == VIDE)
      {
            Case[x][y] = TOMBEALEAU;
            refreshGrille(x, y, x + 1, y + 1);
            dernierePositionGagnante[0] = -1;
            dernierePositionGagnante[1] = -1;

            return 0;
      }
      else if (Case[x][y] == NAVIRE)
      {
            Case[x][y] = TOUCHE;
            caseRestantes[Case2[x][y]]--;
            if (estCoule(Case2[x][y]) == true)
            {
                  refreshGrille(posNavire[Case2[x][y]][0], posNavire[Case2[x][y]][1], W, H);
                  // if (!IA)
                  {
                        focusnavire = findFocus();
                  }
                  dernierePositionGagnante[0] = x;
                  dernierePositionGagnante[1] = y;
                  dernierTir[0] = x;
                  dernierTir[1] = y;
                  naviresRestants--;
                  if (estMort())
                  {
                        return 3;
                  }
                  else
                  {
                        return 2;
                  }
            }
            else
            {
                  refreshGrille(x, y, x + 1, y + 1);
                  {
                        focusnavire = Case2[x][y];
                        focusx = x;
                        focusy = y;
                  }
                  dernierePositionGagnante[0] = x;
                  dernierePositionGagnante[1] = y;
                  dernierTir[0] = x;
                  dernierTir[1] = y;
                  return 1;
            }
      }

      return 0;
}

void Grille::zoneFocus(int &x, int &y)
{
      if (aDesVoisins(x, y))
      {
            while (!caseNonDecouverte(x, y))
            {
                  switch (difficulte)
                  {
                  case 2 ... 3:
                        x = rand() % (toucheBord(focusx, W) ? 2 : 3) + (max(focusx - 1, 0));
                        y = rand() % (toucheBord(focusy, H) ? 2 : 3) + (max(focusy - 1, 0));
                        break;
                  case 1:
                        x = rand() % (toucheBord(focusx, W) ? 3 : toucheBord(focusx + 1, W) ? 4 : 5) + (max(focusx - 2, 0));
                        y = rand() % (toucheBord(focusy, H) ? 3 : toucheBord(focusy + 1, H) ? 4 : 5) + (max(focusy - 2, 0));
                        break;
                  }
            }
      }
      else
      {
            focusnavire = findFocus();
            if (focusnavire != -1)
            {
                  x = focusx;
                  y = focusy;
                  zoneFocus(x, y);
            }
            else
            {
                  return;
            }
      }
}

bool Grille::aDesVoisins(int x, int y)
{

      int debutX = max(0, x - 1);
      int debutY = max(0, y - 1);

      int finX = min(x + 1, W - 1);
      int finY = min(y + 1, H - 1);

      while (debutX <= finX)
      {
            int debutY = max(0, y - 1);

            while (debutY <= finY)
            {
                  if (caseNonDecouverte(debutX, debutY))
                  {
                        return true;
                  }
                  debutY++;
            }
            debutX++;
      }
      return false;
}

bool Grille::caseNonDecouverte(int x, int y)
{

      return (Case[x][y] == VIDE || Case[x][y] == NAVIRE);
}

int Grille::findFocus()
{
      for (int x = 0; x < W; x++)
      {
            for (int y = 0; y < H; y++)
            {
                  if (Case[x][y] == TOUCHE && aDesVoisins(x, y))
                  {

                        focusx = x;
                        focusy = y;
                        return Case2[x][y];
                  }
            }
      }

      return -1;
}

void Grille::caseAleatoire(int &x, int &y)
{

      x = rand() % W;
      y = rand() % H;
      int k = 50;

      while (Case[x][y] == TOUCHE || Case[x][y] == COULE || Case[x][y] == TOMBEALEAU)
      {
            srand((int)time(0) + k);

            x = rand() % W;
            y = rand() % H;
            k += 50;
      }
}

string myitoa(int i)
{
      std::ostringstream s;
      s << i;
      const std::string i_as_string(s.str());
      return i_as_string;
}

void Grille::setEstCible(bool c)
{

      // Si le joueur est cible et ne joue pas
      if (c && !joue)
      {
            fenetre.setCouleurBordure(BRED);

            mvwprintw(fenetre.frame, H + 1, 1 + W - nom.length() / 2, nom.c_str());
            string str = "(" + myitoa(naviresRestants) + "N)";
            mvwprintw(fenetre.frame, H + 1, 2 * W - 2, str.c_str());

            wrefresh(fenetre.frame);
      }
      else if (c && joue)
      // Si le joueur se cible lui-même
      {
            fenetre.setCouleurBordure(BCYAN);
            // wattron(fenetre.frame,COLOR_PAIR(BBLUE));
            mvwprintw(fenetre.frame, H + 1, 1 + W - nom.length() / 2, nom.c_str());
            string str = "(" + myitoa(naviresRestants) + "N)";
            mvwprintw(fenetre.frame, H + 1, 2 * W - 2, str.c_str());
            wrefresh(fenetre.frame);
      }
      else if (!c && joue)
      // Si le joueur n'est plus ciblé mais est toujours joueur
      {
            fenetre.setCouleurBordure(BBLUE);

            mvwaddch(fenetre.frame, H + 1, 1 + W - nom.length() / 2 - 2, ACS_BULLET);
            mvwaddch(fenetre.frame, H + 1, 1 + W + nom.length() / 2 + 2, ACS_BULLET);
            mvwprintw(fenetre.frame, H + 1, 1 + W - nom.length() / 2, nom.c_str());
            string str = "(" + myitoa(naviresRestants) + "N)";
            mvwprintw(fenetre.frame, H + 1, 2 * W - 2, str.c_str());
            wrefresh(fenetre.frame);
      }
      else if (!c)
      // Si le joueur n'est plus ciblé
      {
            fenetre.setCouleurBordure(bordure);
            // wattron(fenetre.frame,COLOR_PAIR(BBLUE));
            mvwprintw(fenetre.frame, H + 1, 1 + W - nom.length() / 2, nom.c_str());
            string str = "(" + myitoa(naviresRestants) + "N)";
            mvwprintw(fenetre.frame, H + 1, 2 * W - 2, str.c_str());
            wrefresh(fenetre.frame);
      }
}

bool Grille::estIA()
{
      return IA;
}

void Grille::setJoue(bool c)
{

      if (c)
      {
            joue = true;
            fenetre.setCouleurBordure(BCYAN);
            mvwaddch(fenetre.frame, H + 1, 1 + W - nom.length() / 2 - 2, ACS_BULLET);
            mvwaddch(fenetre.frame, H + 1, 1 + W + nom.length() / 2 + 2, ACS_BULLET);

            mvwprintw(fenetre.frame, H + 1, 1 + W - nom.length() / 2, nom.c_str());
            string str = "(" + myitoa(naviresRestants) + "N)";
            mvwprintw(fenetre.frame, H + 1, 2 * W - 2, str.c_str());
            wrefresh(fenetre.frame);
      }
      else
      {
            joue = false;
            fenetre.setCouleurBordure(bordure);
            mvwprintw(fenetre.frame, H + 1, 1 + W - nom.length() / 2, nom.c_str());
            string str = "(" + myitoa(naviresRestants) + "N)";
            mvwprintw(fenetre.frame, H + 1, 2 * W - 2, str.c_str());
            wrefresh(fenetre.frame);
      }
}

bool Grille::getJoue()
{
      return joue;
}

void Grille::checkSpeedG()
{
      int ch = getch();

      switch (ch)
      {
      case '+':
            vitesse = min(vitesse + 1, 9);
            break;
      case '-':
            vitesse = max(vitesse - 1, 0);
            break;
      case 'm':
            retourmenu = true;
            fenetre.print(0, 0, "Veuillez patientez...", RBLACK);
            break;
      case 's':
            retoursauvegarde = true;
            fenetre.print(0, 0, "Veuillez patientez...", RBLACK);
            break;
      }
      (*aide).print((*aide).getWindowWidth() - 2, 0, myitoa(vitesse), CBLACK);
}

void Grille::deplacementIA(int dx, int dy, int x, int y)
{

      double distance;
      double distance2;
      while ((distance = (double)sqrt(pow(x - dx, 2)) + pow(y - dy, 2)) > 0)
      {
            if (checkCheminHorizontal(dx, dy, x))
            {
                  do
                  {

                        if (dx < x)
                        {
                              caseSuivante(dx, dy);
                              wait();
                        }
                        else
                        {
                              moveLeft(dx, dy);
                              wait();
                        }
                  } while (dx != x);
                  continue;
            }
            if (checkCheminVertical(dx, dy, y))
            {
                  do
                  {

                        if (dy > y)
                        {
                              moveUp(dx, dy);
                              wait();
                        }
                        else
                        {
                              moveDown(dx, dy);
                              wait();
                        }
                  } while (dy != y);
                  continue;
            }

            if (dx < x)
            {
                  caseSuivante(dx, dy);
                  wait();
            }
            else if (dx > x)
            {
                  casePrecedente(dx, dy);
                  wait();
            }

            if (dy < y)
            {
                  moveDown(dx, dy);
                  wait();
            }
            else if (dy > y)
            {
                  moveUp(dx, dy);
                  wait();
            }
            if (distance2 >= distance)
            {
                  if (dy < y)
                  {
                        while (dx != x && dy != y)
                        {
                              caseSuivante(dx, dy);
                              wait();
                        }
                  }
                  else
                  {
                        while (dx != x && dy != y)
                        {
                              casePrecedente(dx, dy);
                              wait();
                        }
                  }
            }
            distance2 = distance;
      }
}

bool Grille::checkCheminVertical(int dx, int dy, int y)
{
      return (caseNonDecouverte(dx, y) && dy != y);
}

bool Grille::checkCheminHorizontal(int dx, int dy, int x)
{
      return (caseNonDecouverte(x, dy) && dx != x);
}

void Grille::wait()
{
      for (int i = 1; i <= 5; i++)
      {
            checkSpeedG();
            usleep((9 - vitesse) * (9 - vitesse) * 2000);
      }
}

void Grille::mauvaiseSelection()
{
      fenetre.setCouleurBordure(BYELLOW);
      if (estMort())
      {
            fenetre.print(0, H / 2 - 2, "Ce joueur est mort", RBLACK);
      }
      else
      {
            fenetre.print(0, H / 2 - 2, "Impossible de vous viser!", RBLACK);
      }
      fenetre.setCarBordure(carGrilleMauvaiseSelection);
      usleep(1000000);
      refreshGrille(0, H / 2 - 2, W, H / 2 + 1);
      fenetre.setCarBordure(carBordureGrille);
      fenetre.setCouleurBordure(BCYAN);
}

int Grille::getScore()
{
      return Score.getScore();
}

bool Grille::toucheBord(int x, int d)
{
      return (d - x == 1 || x == 0);
}

int *Grille::getDernierePositionGagnante()
{
      return dernierePositionGagnante;
}

int *Grille::getDernierTir()
{
      return dernierTir;
}

bool Grille::estMort()
{
      return naviresRestants == 0;
}

void Grille::croixmort()
{

      fenetre.clear();
      for (int i = 0; i < W; i++)
      {
            fenetre.print(2 * i, i, ' ', BRED);
            fenetre.print(2 * i + 1, i, ' ', BRED);
            fenetre.print(2 * W - 2 - 2 * i, i, ' ', BRED);
            fenetre.print(2 * W - 2 - 2 * i + 1, i, ' ', BRED);
      }
}

void Grille::SauvegarderGrille(fstream &save)
{
      for (int i = 0; i < H; i++)
      {
            for (int j = 0; j < W; j++)
            {
                  save << Case[j][i] << ':';
            }
            save << endl;
      }

      save << "-----------------------" << endl;

      for (int i = 0; i < H; i++)
      {
            for (int j = 0; j < W; j++)
            {
                  save << Case2[j][i] << ':';
            }
            save << endl;
      }
}

void Grille::getCasesRestantes(fstream &save)
{
      for (int i = 0; i < 5; i++)
      {
            save << caseRestantes[i] << ':';
      }
}

int Grille::getNaviresRestants()
{
      return naviresRestants;
}

void Grille::getPosNavires(fstream &save)
{
      for (int i = 0; i < 5; i++)
      {
            for (int j = 0; j < 2; j++)
            {
                  save << posNavire[i][j] << ':';
            }
            save << ':';
      }
}

void Grille::ChargementDonnees(int **Case, int **Case2, int *casesRestantes, int **posNavires, int naviresRestants, int *NbP, int MissilesTires, int MissilesGagnants, int tailleFlotte)
{
      for (int x = 0; x < W; x++)
      {
            for (int y = 0; y < H; y++)
            {

                  this->Case[x][y] = (Case[x][y] == 0 ? VIDE : (Case[x][y] == 1 ? TOMBEALEAU : (Case[x][y] == 2 ? NAVIRE : (Case[x][y] == 3 ? TOUCHE : COULE))));
                  this->Case2[x][y] = Case2[x][y];
            }
      }
      this->naviresRestants = naviresRestants;
      Score.ChargerDonnees(MissilesTires, MissilesGagnants, tailleFlotte);
      int a = 0, b = 0;
      for (int n = 0; n < 5; n++)
      {
            this->posNavire[n][0] = posNavires[n][0];
            this->posNavire[n][1] = posNavires[n][1];
            this->caseRestantes[n] = casesRestantes[n];
            flotte.estAuPort(n, false);
            for (int i = 0; i < NbP[n]; i++)
            {

                  pivoteDroite(n, a, b);
            }
      }
      if (!estMort())
            for (int x = 0; x < W; x++)
            {
                  {
                        for (int y = 0; y < H; y++)
                        {
                              switch (Case[x][y])
                              {
                              case VIDE:
                                    fenetre.print(2 * x, y, ' ', colVide);
                                    fenetre.print(2 * x + 1, y, ' ', colVide);
                                    break;
                              case TOMBEALEAU:
                                    fenetre.print(2 * x, y, ' ', colManque);
                                    fenetre.print(2 * x + 1, y, ' ', colManque);
                                    break;
                              case TOUCHE:
                                    fenetre.print(2 * x, y, '#', colTouche);
                                    fenetre.print(2 * x + 1, y, '#', colTouche);
                                    break;
                              case COULE:
                                    fenetre.print(2 * x, y, '#', colCoule);
                                    fenetre.print(2 * x + 1, y, '#', colCoule);
                                    break;
                              }
                        }
                  }
            }
      else
      {
            croixmort();
      }

      focusnavire = findFocus();
}

int Grille::getNbPivotements(int n)
{
      return NombrePivotements[n];
}