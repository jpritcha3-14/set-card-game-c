snakegame : src/set.c src/helper_functions.c src/main.c
	gcc -std=gnu11 -Wall -Iheaders src/set.c src/helper_functions.c src/main.c src/menu.c src/timer.c -o set -lncurses -lpthread -lsqlite3

clean :
	rm set
