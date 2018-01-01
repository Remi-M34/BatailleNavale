
all: BatailleNavale

BatailleNavale: obj/window.o obj/grille.o obj/flotte.o obj/config.o obj/message.o obj/navire.o obj/menu.o obj/jeu.o obj/scores.o
	g++ -g $^ -o $@ -lncurses -W -ansi -g

obj/%.o: src/%.cpp include/%.h
	g++ -c $< -o $@ -lncurses -W -ansi -g

clean:
	rm -rf obj/*.o

mrproper: clean
	rm -rf BatailleNavale
			