main: main.c dictionary.c game.c
	gcc -g -O3 main.c dictionary.c game.c -lncurses -lcurl -o main
