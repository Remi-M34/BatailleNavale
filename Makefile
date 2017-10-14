
all: prog

prog: window.o test-piece.o
	g++ $^ -o $@ -lncurses -Wall


%.o: %.cpp
	g++ -c $^ -lncurses -Wall

