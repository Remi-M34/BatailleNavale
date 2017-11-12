
all: prog

prog: window.o test-piece.o grille.o flotte.o
	g++ $^ -o $@ -lncurses -W -Wall -ansi -pedantic



%.o: %.cpp
	g++ -c $^ -lncurses -W -Wall -ansi -pedantic


