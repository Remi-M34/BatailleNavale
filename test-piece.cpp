#include "window.h"


void myprogram(){
  int ch;
  int h=10,w=10;
  Window menu(3,30,1,0);
  Window plateau(h,w,1,6);
  menu.setCouleurBordure(BRED);
  plateau.setCouleurBordure(BBLUE);
  
  menu.print(1,1,"Tapez q pour quitter !!!",WRED);
  
  int x=w/2,y=h/2;
  char p='X';
  Color col=WBLUE;
  plateau.print(x,y,p,col);
  
  while((ch = getch()) != 'q')
    {
      switch (ch) {
      case '1':
	col=BMAGENTA;
	break;
      case '2':
	col=WCYAN;
	break;
      case 'c':
	plateau.clear();
	break;
      case KEY_UP:
	plateau.print(x,y,' ');
	plateau.print(x,--y,p,col);
	break;
      case KEY_DOWN:
	plateau.print(x,y,' ');
	plateau.print(x,++y,p,col);
	break;
      case KEY_LEFT:
	plateau.print(x,y,' ');
	plateau.print(--x,y,p,col);
	break;
      case KEY_RIGHT:
	plateau.print(x,y,' ');
	plateau.print(++x,y,p,col);
	break;
      case '\n':
	x=w/2,y=h/2;
	plateau.print(x,y,p,col);
	break;
      case '\t':
	Color tmp= menu.getCouleurBordure();
	menu.setCouleurBordure(plateau.getCouleurBordure());
	plateau.setCouleurBordure(tmp);
	break;
      }
    }
}

int main(){
  startProgramX();
  myprogram();
  stopProgramX();
  return 0;
}
