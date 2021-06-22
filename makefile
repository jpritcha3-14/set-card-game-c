snakegame : src/set.c src/helper_functions.c src/main.c
	gcc -std=gnu11 -Wall -Iheaders src/set.c src/helper_functions.c src/main.c -o set -lncurses -lsqlite3

clean :
	rm set
