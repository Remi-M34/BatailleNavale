#include "grille.h"
#include "window.h"
#include "flotte.h"



Grille::Grille(int h, int w, int sx, int sy) : height(h), width(w), startx(sx), starty(sy), fenetre(h,w,sx,sy), flotte(5,14,sx+12,sy+3){}

Grille::~Grille(){}


void Grille::placement(int t){
    int ch;

    while((ch = getch()) != 'q' && ch = getch()) != '\n'){


    switch (ch) {
      case '1':
	col=BMAGENTA;
	break;
      case '2':
	col=WCYAN;
	break;
      case 'c':
	plateaujoueur.fenetre.clear();
	break;
      case KEY_UP:
      if (y!=0){
	plateaujoueur.fenetre.print(x,y,' ');
	plateaujoueur.fenetre.print(x2,y,' ');
	plateaujoueur.fenetre.print(x,--y,p,col);
      plateaujoueur.fenetre.print(x2,y,p,col);}
	break;
      case KEY_DOWN:
      if (y!=9){
	plateaujoueur.fenetre.print(x,y,' ');
	plateaujoueur.fenetre.print(x2,y,' ');
	plateaujoueur.fenetre.print(x,++y,p,col);
	plateaujoueur.fenetre.print(x2,y,p,col);}
	break;
      case KEY_LEFT:
           if (x!=0){
      plateaujoueur.fenetre.print(x,y,' ');
      plateaujoueur.fenetre.print(x2,y,' ');
      x-=2;
      x2-=2;
      plateaujoueur.fenetre.print(x,y,p,col);	
      plateaujoueur.fenetre.print(x2,y,p,col);}
	break;
      case KEY_RIGHT:
           if (x2!=19){
	plateaujoueur.fenetre.print(x,y,' ');
	plateaujoueur.fenetre.print(x2,y,' ');
      x+=2;
      x2+=2;
	plateaujoueur.fenetre.print(x,y,p,col);
	plateaujoueur.fenetre.print(x2,y,p,col);}
	break;
    }

    }

}