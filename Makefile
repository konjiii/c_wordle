main: main.c randomword.c game.c
	gcc -g main.c randomword.c game.c -lncurses -lcurl -o main
