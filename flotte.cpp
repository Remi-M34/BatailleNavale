#include "flotte.h"
#include "window.h"


Flotte::Flotte(int h, int w, int sx, int sy) : height(h), width(w), startx(sx), starty(sy), fenetre(h,w,sx,sy){}

Flotte::~Flotte(){}


void Flotte::present(){
    if (n[0]){
        for (int i = 0 ; i  < 5 ; i++){
            fenetre.print(0,i,s[0],BGREEN);
            fenetre.print(1,i,s[0],BGREEN);
            }
        }

    if (n[1]){
        for (int i = 0 ; i  < 4 ; i++){
            fenetre.print(3,i,s[1],BYELLOW);
            fenetre.print(4,i,s[1],BYELLOW);
          }
        }


    if (n[2]){
        for (int i = 0 ; i  < 3 ; i++){
            fenetre.print(6,i,s[2],BMAGENTA);
            fenetre.print(7,i,s[2],BMAGENTA);
            }
        }

    if (n[3]){
        for (int i = 0 ; i  < 3 ; i++){
            fenetre.print(9,i,s[3],BCYAN);
            fenetre.print(10,i,s[3],BCYAN);
             }
        }

    if (n[4]){
        for (int i = 0 ; i  < 2 ; i++){
            fenetre.print(12,i,s[4],BWHITE);
            fenetre.print(13,i,s[4],BWHITE);
            }
        }
}



void Flotte::selection(int i){
    s[i] = ' ';
    i++;
    while (n[(i%5)] != 1 && s[(i%5)] != ' '){
        i+=1;

    }

    s[i] = '#';

    present();

}


void Flotte::first (){

}
    