#include "window.h"
#include "grille.h"
#include "flotte.h"
#include <unistd.h>


void myprogram(){
  int ch;
  int h=10,w=20;
  Window menu(3,30,1,0);
 
  Grille plateaujoueur(h,w,1,6);  
  Window plateau2(h,w,25,6);
  Flotte flottejoueur(5,14,1,18);
  menu.setCouleurBordure(BRED);  

  plateaujoueur.fenetre.setCouleurBordure(BBLUE);
  

  menu.print(1,1,"Tapez q pour quitter !!!",WRED);
  
  int x=w/2,x2=x+1, y=h/2;
  char p=' ';
  Color col=WBLUE;
//  flottejoueur.fenetre.print(x,y,p,col);
//  flottejoueur.fenetre.print(x2,y,p,col);
        flottejoueur.present();

        int s = 0;

  while((ch = getch()) != 'q')
    {




//PHASE 1  : placement des navires
      if (flottejoueur.places == 0){


      switch (ch) {
            case KEY_LEFT:
                  flottejoueur.selection(s%5, 0);
                  if (s == 0)
                        s=4;
                  else
                        s--;
                  break;
            case KEY_RIGHT:
                  flottejoueur.selection(s%5, 1);
                  s++;
                  break;
            
            case '\n':
                  int i = 0;
                  while (flottejoueur.n[i] != 1 && flottejoueur.s[i] != '#' && i < 5){
                        i++;
                  }
                  grillejoueur.placement(flottejoueur.taille(i))

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
      case '\n':
      plateaujoueur.fenetre.print(x,y,' ');
	plateaujoueur.fenetre.print(x2,y,' ');
	x=w/2, x2=x+1, y=h/2;
	plateaujoueur.fenetre.print(x,y,p,col);
	plateaujoueur.fenetre.print(x2,y,p,col);
	break;
      case '\t':
	Color tmp= menu.getCouleurBordure();
	menu.setCouleurBordure(plateaujoueur.fenetre.getCouleurBordure());
	plateaujoueur.fenetre.setCouleurBordure(tmp);
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
