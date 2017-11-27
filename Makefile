
all: prog

prog: obj/window.o obj/test-piece.o obj/grille.o obj/flotte.o obj/config.o obj/message.o obj/navire.o obj/menu.o obj/jeu.o
	g++ $^ -o $@ -lncurses -W -Wall -ansi -pedantic



obj/%.o: src/%.cpp
	g++ -c $^ -o $@ -lncurses -W -Wall -ansi -pedantic


