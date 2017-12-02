#include "../include/headers.h"
using namespace std;
void options();

#define H getHeightGrille()
#define W getWidthGrille()
#define WF getDimFlotte('w')
#define WH getDimFlotte('h')

void myprogram()
{
  clear();

  refresh();


  int ch;
  int const h = 10;
  int const w = 20;

noecho();
Jeu Jeu2(3,1);


Jeu2.placement();


  while (1)
  {
refresh();
  }
  // Création des grilles et flottes dont les positions dépendent des dimensions calculées dans le fichier config
  // Grille joueur(6, 9, 6, 12 + getHeightGrille());

  // Grille joueur[2] = Grille joueur(5,5,3,3);

  // Grille ennemi(10 + 2 * max(WF, getWidthGrille()), 9, 10 + 2 * max(WF, getWidthGrille()),  12 + getHeightGrille());

  // joueur.flotte.initSelection();

  // menu.print(0, 0, insPlacement(), WBLACK);
  // joueur.selectionNavire();

  // while ((ch = getch()) != 'q')
  // {
  // }
}

// Grille* creationGrilles(int n, int j)
// {

//   for (int i = 0 ; i < n ; i++)
//   {
//     if (j > 0)
//     {
//       Grille Grilles i(6+20*i, 9, 6, 12 + getHeightGrille());

//     }
//   }
//   return Grille;
// }

int main()
{

  //     fstream config("config.txt", ios::in | ios::out | ios::trunc);
  //     config << "15x15\nxxxxx\n\nx\nx\nx\nx\nx\n\nx\n\nxx\nxx\n\nxxx\n x";

  // config.close();

  // int **n = new int *[3];

  // for (int i = 0; i < 3; i++)
  // {
  //   n[i] = new int[4];
  //   n[i][0] = 1;
  //   n[i][1] = 1;
  //   n[i][2] = 1;
  //   n[i][3] = 1;
  // }

  // modifierNavires(2, 3, 4, n);

  Menu menu;
  menu.mainMenu();

  startProgramX();
  myprogram();
  stopProgramX();

  return 0;
}
