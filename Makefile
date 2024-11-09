main: main.c randomword.c game.c
	gcc -g -O3 main.c randomword.c game.c -lncurses -lcurl -o main
