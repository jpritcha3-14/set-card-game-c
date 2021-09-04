# SET
The SET card game implemented in C using ncurses to draw the game in a terminal, pthreads to add a timer, and sqlite3 for a leaderboard so you can see your fastest 10 times solving the deck, or compete with friends on the same machine.  For details on how to play SET, or to play SET in your browser, see [my other implementation of SET in JavaScript](https://github.com/jpritcha3-14/set-card-game).

![menu png](https://github.com/jpritcha3-14/set-card-game-c/blob/master/assets/menu.png) ![game png](https://github.com/jpritcha3-14/set-card-game-c/blob/master/assets/game.png)

## Install
The game requires ncurses and sqlite3 which are available on most any Unix-like system.  To install them on a Debian based distro use: ```sudo apt-get install libncurses5-dev libncursesw5-dev sqlite3 libsqlite3-dev```

Then use the makefile to build and install:
```
make
sudo make install
```

## Uninstall
```
sudo make clean
```

## Gameplay
- Find and select 3 cards that form a SET
  - If the cards form a valid set, 3 more cards are dealt from the deck
- If there are no SETS on the board, the deck and board are shuffled together and 12 new cards are dealt
- The game ends when there are no more SETS on the board or in the deck
- The number of cards left in the deck and number of SETS on the board are displayed below the board
- Messages and the game timer are displayed above the board


## Controls
#### Menu
- **w/s**: Move cursor
- **space**: Select

#### Game
- **w/a/s/d**: Change the highlighted card 
- **space**: Select or deselect the hilighted card
- **space**: Continue when a message is displayed (that's a set, not a set, shuffling, no more sets)
- **q**: Quit to the menu

#### Leaderboard 
- Enter a name of up to 10 characters and then press enter to save the leaderboard time and return to the menu
- Press any key to return to the menu when viewing high scores
