#include "../include/headers.h"


using namespace std;
#define H getHeightGrille()
#define W getWidthGrille()

Grille::Grille(int const sx, int const sy, int sxf, int syf) : fenetre(H, W * 2, sx, sy), flotte(sxf, syf, 1)
{

      //Initialise la grille et rend toutes les cases de la grille VIDE
      initCouleurs();
      init();
}

Grille::Grille(int const sx, int const sy, int sxf, int syf, bool estIA) : fenetre(H, W * 2, sx, sy), flotte(sxf, syf, 1), IA(estIA)
{

      //Initialise la grille et rend toutes les cases de la grille VIDE
      initCouleurs();
      init();
}

Grille::~Grille() {}

void Grille::selectionNavire()
{
      // Ré-initialisation des couleurs car bug sinon...
      initCouleurs();

      //Si le joueur est une IA
      if (IA)
      {
            placementAleatoire();
            return;
      }

      int ch;
      int n = flotte.getPremierNavire();
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

            case '\n':
                  flotte.estAuPort(n, false);
                  placementNavire(n);

                  if (flotte.getEstAuPort(n))
                  {
                        break;
                  }

                  flotte.initSelection();
                  n = flotte.getPremierNavire();

                  if (n == -1)
                  {
                        colNavires = colVide;
                        refreshGrille(0, 0, W, H);
                        return;
                  }
                  // flotte.echangeSelection(n, flotte.getPremierNavire());
                  flotte.refreshPort(0);
                  refreshGrille(0, 0, 0, 0);

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
                  Case2[i][j] = -1;
                  fenetre.print(2 * i, j, ' ');
                  fenetre.print(2 * i + 1, j, ' ');
                  usleep(300);
            }
      }
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

void Grille::refreshNavireGrille(int n, int fx, int fy)
{

      // if (maxX == 0 && maxY == 0)
      // {
      //       maxX = min(W, fx + 7);
      //       maxY = min(H, fy + 7);
      // }
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
      refreshGrille(0, 0, 0, 0);
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

            case '\n':

                  if (checkPlacement(n, x, y))
                  {
                        validerNavire(n, x, y);

                        refreshGrille(0, 0, 0, 0);
                        return;
                  }
                  else
                  {
                        fenetre.setCouleurBordure(colMauvaiseCouleur);
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
      fenetre.setCouleurBordure(bordure);
      fenetre.setCarBordure(carBordureGrille);

      int x = 0;
      int y = 0;
      char c = ' ';
      char c2 = ' ';
      if (!checkPlacement(n, sx, sy))
      {
            c = '#';
      }

      // refreshGrille(sx - 1, sy - 1, 0, 0);
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

bool Grille::check(int n, int sx, int sy)
{
      int x = 0;
      int y = 0;

      for (x = 0; x < flotte.getWidthnavire(n); x++)
      {
            for (y = 0; x < flotte.getHeightnavire(n); y++)
            {
            }
      }
}

void Grille::pivoteDroite(int n, int &x, int &y)
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

      // refreshGrille();

      flotte.swapDimensionsNavire(n);

      if (x + flotte.getWidthnavire(n) > W)
      {
            x -= abs(flotte.getHeightnavire(n) - flotte.getWidthnavire(n));
      }
      if (y + flotte.getHeightnavire(n) > H)
      {
            y -= abs(flotte.getHeightnavire(n) - flotte.getWidthnavire(n));
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
                  }
            }
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

      if (IA)
      {
            return destinationMissileAleatoire();
      }

      int ch;
      int x = W / 2;
      int y = H / 2;

      findMilieu(x, y);
      fenetre.print(2 * x, y, ' ', colCaseSelectionnee);
      fenetre.print(2 * x + 1, y, ' ', colCaseSelectionnee);

      while ((ch = getch()) != 'q')
      {
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
            case '\n':
                  if (Case[x][y] == VIDE)
                  {
                        Case[x][y] = TOMBEALEAU;
                        refreshGrille(x, y, x + 1, y + 1);
                        return 0;
                  }
                  else if (Case[x][y] == NAVIRE)
                  {
                        Case[x][y] = TOUCHE;
                        caseRestantes[Case2[x][y]]--;
                        if (estCoule(Case2[x][y]) == true)
                        {
                              refreshGrille(posNavire[Case2[x][y]][0], posNavire[Case2[x][y]][1], W, H);

                              return 2;
                        }
                        else
                        {
                              refreshGrille(x, y, x + 1, y + 1);

                              return 1;
                        }
                  }
                  break;
            }
      }
}

void Grille::moveRight(int &x, int &y)
{

      for (int i = x + 1; i < W; i++)
      {
            if (Case[i][y] == VIDE || Case[i][y] == NAVIRE)
            {
                  refreshGrille(x, y, W, y + 1);
                  fenetre.print(2 * i, y, ' ', colCaseSelectionnee);
                  fenetre.print(2 * i + 1, y, ' ', colCaseSelectionnee);
                  x = i;
                  return;
            }
      }
}

void Grille::moveLeft(int &x, int &y)
{

      for (int i = x - 1; i >= 0; i--)
      {
            if (Case[i][y] == VIDE || Case[i][y] == NAVIRE)
            {
                  refreshGrille(0, y, x + 1, y + 1);
                  fenetre.print(2 * (i), y, ' ', colCaseSelectionnee);
                  fenetre.print(2 * i + 1, y, ' ', colCaseSelectionnee);
                  x = i;
                  return;
            }
      }
}

void Grille::moveUp(int &x, int &y)
{

      for (int i = y - 1; i >= 0; i--)
      {
            if (Case[x][i] == VIDE || Case[x][i] == NAVIRE)
            {
                  refreshGrille(x, 0, x + 1, y + 1);
                  fenetre.print(2 * (x), i, ' ', colCaseSelectionnee);
                  fenetre.print(2 * x + 1, i, ' ', colCaseSelectionnee);
                  y = i;
                  return;
            }
      }
}

void Grille::moveDown(int &x, int &y)
{

      for (int i = y + 1; i < H; i++)
      {
            if (Case[x][i] == VIDE || Case[x][i] == NAVIRE)
            {
                  refreshGrille(x, y, x + 1, H);
                  fenetre.print(2 * (x), i, ' ', colCaseSelectionnee);
                  fenetre.print(2 * x + 1, i, ' ', colCaseSelectionnee);
                  y = i;
                  return;
            }
      }
}

void Grille::findMilieu(int &x, int &y)
{
      int k = 1;
      while (Case[x][y] != VIDE || Case[x][y] != NAVIRE)
      {
            x--;
            y--;

            for (int i = x; i <= x + 2 * k; i++)
            {
                  for (int j = y; j <= y + 2 * k; j++)
                  {
                        if (Case[i][j] == VIDE || Case[i][j] == NAVIRE)
                        {
                              x = i;
                              y = j;
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

// void Grille::test()
// {
//       fstream test("test.txt", ios::in | ios::out | ios::trunc);

//       for (int x = 0; x < W; x++)
//       {
//             for (int y = 0; y < H; y++)
//             {
//                   test << x << ":" << Case[x][y] << "   ";
//             }
//             test << '\n';
//       }

//       test.close();
// }

void Grille::initCouleurs()
{
      ifstream couleurs("config/couleurs.txt", ios::in);
      int lignes = 1;
      string ligne;

      while (getline(couleurs, ligne))
      {

            switch (lignes)
            {
            case 14:
                  fenetre.setCouleurBordure(convertColor(ligne));
                  bordure = convertColor(ligne);
                  break;
            case 15:
                  carBordureGrille = ligne[0];
                  fenetre.setCarBordure(carBordureGrille);
                  break;
            case 16:
                  colMauvaiseCouleur = convertColor(ligne);
                  break;
            case 20:
                  colNavires = convertColor(ligne);
                  break;
            case 25:
                  colCaseSelectionnee = convertColor(ligne);
                  break;
            case 26:
                  colTouche = convertColor(ligne);
                  break;
            case 27:
                  colCoule = convertColor(ligne);
                  break;
            case 28:
                  colManque = convertColor(ligne);
                  break;
            case 29:
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
      int n = flotte.getPremierNavire();
      int x, y, p;
      cout << endl
           << endl;
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
            cout << x << ':';
            cout << y;
            if (checkPlacement(n, x, y))
            {
                  flotte.estAuPort(n, false);
                  validerNavire(n, x, y);
                  flotte.initSelection();
                  n = flotte.getPremierNavire();
            }

            if (n == -1)
            {
                  colNavires = colVide;
                  usleep(1000000);

                  return;
            }
      }
}

int Grille::destinationMissileAleatoire()
{
      srand((int)time(0));
      int x, y;
      if (focus == -1)
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
      else
      {
            zoneFocus(x, y);
      }

      fenetre.print(2 * x, y, ' ', colCaseSelectionnee);
      fenetre.print(2 * x + 1, y, ' ', colCaseSelectionnee);
      usleep(500000);

      if (Case[x][y] == VIDE)
      {
            Case[x][y] = TOMBEALEAU;
            refreshGrille(x, y, x + 1, y + 1);
            return 0;
      }
      else if (Case[x][y] == NAVIRE)
      {
            Case[x][y] = TOUCHE;
            caseRestantes[Case2[x][y]]--;
            if (estCoule(Case2[x][y]) == true)
            {
                  refreshGrille(posNavire[Case2[x][y]][0], posNavire[Case2[x][y]][1], W, H);
                  focus = -1;
                  return 2;
            }
            else
            {
                  refreshGrille(x, y, x + 1, y + 1);
                  focus = Case2[x][y];
                  return 1;
            }
      }
}

void Grille::zoneFocus(int &x, int &y)
{
      x = rand() % 9 + (max(x - 4, 0));
      y = rand() % 9 + (max(y - 4, 0));
}