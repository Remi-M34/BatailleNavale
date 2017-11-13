#include "window.h"
#include "grille.h"
#include "plateau.h"
#include "flotte.h"
#include <unistd.h>
using namespace std;


void myprogram(){
  int ch;
  int const h=10;
  int const w=20;
  Window menu(3,30,1,0);
  Plateau plateau(h,w,6,6,2);


  menu.setCouleurBordure(BRED);  

  plateau.joueur.fenetre.setCouleurBordure(BBLUE);

  menu.print(1,1,"Tapez q pour quitter !!!",WRED);
  
  int x=w/2,x2=x+1, y=h/2;
  char p=' ';
  Color col=WBLUE;
//  flottejoueur.fenetre.print(x,y,p,col);
//  flottejoueur.fenetre.print(x2,y,p,col);
        plateau.joueur.flotte.present();

        int s = 0;

  while((ch = getch()) != 'q')
    {




//PHASE 1  : placement des navires
      if (plateau.joueur.flotte.places == 0){


      switch (ch) {
            case KEY_LEFT:
                  plateau.joueur.flotte.selection(s%5, 0);
                  if (s == 0)
                        s=4;
                  else
                        s--;
                  break;
            case KEY_RIGHT:
                  plateau.joueur.flotte.selection(s%5, 1);
                  s++;
                  break;
            
            case '\n':
                  int i = 0;
                  while (!(plateau.joueur.flotte.selected(i)) && i < 5){
                        i++;
                  }
                  plateau.joueur.placement(plateau.joueur.flotte.taille(i));

                  break;
            }


      }
//PHASE 2

      else{
      switch (ch) {
      case '1':
	col=BMAGENTA;
	break;
      case '2':
	col=WCYAN;
	break;
      case 'c':
	plateau.joueur.fenetre.clear();
	break;
      case KEY_UP:
      if (y!=0){
	plateau.joueur.fenetre.print(x,y,' ');
	plateau.joueur.fenetre.print(x2,y,' ');
	plateau.joueur.fenetre.print(x,--y,p,col);
      plateau.joueur.fenetre.print(x2,y,p,col);}
	break;
      case KEY_DOWN:
      if (y!=9){
	plateau.joueur.fenetre.print(x,y,' ');
	plateau.joueur.fenetre.print(x2,y,' ');
	plateau.joueur.fenetre.print(x,++y,p,col);
	plateau.joueur.fenetre.print(x2,y,p,col);}
	break;
      case KEY_LEFT:
           if (x!=0){
      plateau.joueur.fenetre.print(x,y,' ');
      plateau.joueur.fenetre.print(x2,y,' ');
      x-=2;
      x2-=2;
      plateau.joueur.fenetre.print(x,y,p,col);	
      plateau.joueur.fenetre.print(x2,y,p,col);}
	break;
      case KEY_RIGHT:
           if (x2!=19){
	plateau.joueur.fenetre.print(x,y,' ');
	plateau.joueur.fenetre.print(x2,y,' ');
      x+=2;
      x2+=2;
	plateau.joueur.fenetre.print(x,y,p,col);
	plateau.joueur.fenetre.print(x2,y,p,col);}
	break;
      case '\n':
      plateau.joueur.fenetre.print(x,y,' ');
	plateau.joueur.fenetre.print(x2,y,' ');
	x=w/2, x2=x+1, y=h/2;
	plateau.joueur.fenetre.print(x,y,p,col);
	plateau.joueur.fenetre.print(x2,y,p,col);
	break;
      case '\t':
	Color tmp= menu.getCouleurBordure();
	menu.setCouleurBordure(plateau.joueur.fenetre.getCouleurBordure());
	plateau.joueur.fenetre.setCouleurBordure(tmp);
	break;
      }}

      if (x == 2)
      {       Window menu2(3,10,20,20);
            menu2.setCouleurFenetre(BWHITE);
      menu2.print(1,1,"x == 2");
      }
      
    }
}

int main(){

  startProgramX();
  myprogram();
  stopProgramX();
  return 0;
}
