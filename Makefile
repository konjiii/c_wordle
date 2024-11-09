main: main.c wordapi.c game.c
	gcc -g -O3 main.c wordapi.c game.c -lncurses -lcurl -o main
