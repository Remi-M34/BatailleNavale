#ifndef __JEU_H
#define __JEU_H

#include "window.h"
#include "config.h"
#include "grille.h"
extern "C" {
#include <curses.h>
}
#include <string>



class Jeu{

public:

  Jeu(int nbjoueurs, int humains);

  //destructeur
  ~Jeu();

void  placement();
void  placementDesNavires();

void initCouleurs();

void initDim2(int j);
bool estIA();
void selectionCible(int c);
void selectUp();
void selectDown();
void selectRight();
void selectLeft();
void joueurSuivant();
int attaque();
void selectionCibleAleatoire();
void deplacementIA();

private:
      Window score;
      Window aide;
      Window bordure;
      int humains;
      int nbjoueurs;
      int vitesse = 3;
      int joueur = 0;
      int cible;
      int cibleSelectionnee = 0;
      int *payback[];

      
      Grille *Joueur[];

      int scoreWidth;
      int syf2;
      int sxf2;
      int sy2, sx2;

      int scoreJoueur1 = 0;
      int scoreJoueur2 = 0;


      Color colScore;
      Color colAide;


     int sx,sy,sxf,syf;

};

int getScoreStartX(int t);
int getBordureWidth(int nbjoueurs);
int getBordureHeight(int nbjoueurs);
int getBordureStartY(int nbjoueurs);
int getAideStartY(int nbjoueurs);



#endif
