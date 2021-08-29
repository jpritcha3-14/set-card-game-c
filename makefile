snakegame : src/set.c src/helper_functions.c src/main.c
	gcc -std=gnu11 -Wall -DDATABASEPATH=\"/home/jpritcha314/set-game/leaderboard.db\" -Iheaders src/set.c src/helper_functions.c src/main.c src/menu.c src/timer.c src/leaderboard.c -o set -lncurses -lpthread -lsqlite3

clean :
	rm set
