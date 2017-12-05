#include "../include/headers.h"

using namespace std;

// #define xm (COLS-100)/2
// #define ym (LINES-33)/2






int getDim(int** navire, int c)
{
  navire = checkRepositionnement(navire);
  int x,y;
  int w = 0;
  int h = 0;
  for ( x = 0 ; x <5 ; x++)
  {
    for ( y = 0 ; y < 5 ; y++)
    {

      if (navire[x][y] == 1)
      {
        h = fmax(y+1,h);
      }

      if (navire[y][x] == 1)
      {
        w = fmax(y+1,w);
      }
    }

  }


  if (c == 0)
  {

    return w;
  }
  else
  {
    return h;
  }

}




int** checkRepositionnement(int** navire)
{


            int k = 0;
            for (int x = 0; x < 5; x++)
            {
                  k += navire[x][0];
            }
            if (k == 0)
            {
                  navire = repositionnementVertical(navire);
                  navire = checkRepositionnement(navire);
            }
            k = 0;
            for (int y = 0; y < 5; y++)
            {
                  k += navire[0][y];
            }
            if (k == 0)
            {
                  navire = repositionnementHorizontal(navire);
                  navire = checkRepositionnement(navire);
            }
      

      return navire;
}

int** repositionnementHorizontal(int** navire)
{
      int x = 0;
      for (x = 0; x < 4; x++)
      {
            for (int y = 0; y < 5; y++)
            {
                  navire[x][y] = navire[x + 1][y];
            }
      }
      x = 4;
      for (int y = 0; y < 5; y++)
      {
            navire[x][y] = 0;
      }

      return navire;
}

int** repositionnementVertical(int** navire)
{
      int y = 0;
      for (int x = 0; x < 5; x++)
      {
            for (y = 0; y < 4; y++)
            {
                  navire[x][y] = navire[x][y + 1];
            }
      }
      y = 4;
      for (int x = 0; x < 5 ; x++)
      {
            navire[x][y] = 0;
      }

      return navire;
}






void nouveauNavire(int n)
{
  Window creation(5, 10, 0, 0,' ');
  creation.setCouleurBordure(BCYAN);
  int c;
  int k = 0;
  int **Case = new int *[5];
  int x = 2;
  int y = 2;

  for (int i = 0; i < 5; i++)
  {
    Case[i] = new int[5];
    for (int j = 0; j < 5; j++)
    {
      Case[i][j] = 0;
    }
  }

refresh:

  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      switch (Case[i][j])
      {
      case 0:
        creation.print(2 * i, j, ' ');
        creation.print(2 * i + 1, j, ' ');
        break;
      case 1:
        creation.print(2 * i, j, ' ', WGREEN);
        creation.print(2 * i + 1, j, ' ', WGREEN);
        break;
      }
    }
  }

  creation.print(2 * x, y, ' ', WGREEN);
  creation.print(2 * x + 1, y, ' ', WGREEN);

  while ((c = getch()) != 'q')
  {
    switch (c)
    {
    case KEY_RIGHT:
      if (x < 4)
      {
        creation.print(2 * x, y, ' ', WBLACK);
        creation.print(2 * x + 1, y, ' ', WBLACK);
        x++;
        goto refresh;
      }
      break;
    case KEY_LEFT:
      if (x > 0)
      {
        creation.print(2 * x, y, ' ', WBLACK);
        creation.print(2 * x + 1, y, ' ', WBLACK);
        x--;
        goto refresh;
      }
      break;
    case KEY_UP:
      if (y > 0)
      {
        creation.print(2 * x, y, ' ', WBLACK);
        creation.print(2 * x + 1, y, ' ', WBLACK);
        y--;
        goto refresh;
      }
      break;
    case KEY_DOWN:
      if (y < 4)
      {
        creation.print(2 * x, y, ' ', WBLACK);
        creation.print(2 * x + 1, y, ' ', WBLACK);
        y++;
        goto refresh;
      }
      break;
    case ' ':
      if (Case[x][y] == 0)
      {
        Case[x][y] = 1;
        k++;
        goto refresh;
      }
      break;


    case 'r':
      return;
    case '\n':
      if (k > 0)
      {

        Case = checkRepositionnement(Case);
        modifierNavires(n, getDim(Case,0), getDim(Case,1), Case);


        return;
      }
      break;

    case 'q':
      return;
      break;

    case 'd':
    if (k > 0)
    {
              Case = checkRepositionnement(Case);
              goto refresh;

    }

      break;

    }
  }
}
