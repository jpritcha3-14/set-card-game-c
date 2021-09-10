setgame: src/set.c src/helper_functions.c src/main.c src/timer.c src/leaderboard.c
	mkdir -p build/assets
	chmod o+w build/assets
	gcc -std=gnu11 -Wall -Iheaders src/set.c src/helper_functions.c src/main.c src/menu.c src/timer.c src/leaderboard.c -o build/set-game -lncurses -lpthread -lsqlite3
	chmod go+x build/set-game
	cp assets/logo.txt assets/cards.txt build/assets

install: build/set-game build/assets
	mkdir -p /usr/local/share/set-game
	cp -pr build/assets /usr/local/share/set-game
	cp -p build/set-game /usr/local/bin/set-game
	rm -rf build 

clean: 
	rm -rf /usr/local/share/set-game
	rm /usr/local/bin/set-game

