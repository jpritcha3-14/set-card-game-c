#include <ncurses.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "helper_functions.h" 
#include "structs.h"

int main() {
    WINDOW *card_windows[9]; 
    char cards[27][CARD_H][CARD_W];
    int wrows, wcols, gamew, gameh, tlcornerx, tlcornery;

    load_cards(cards, "assets/cards.txt");

    initscr();
    noecho();
    start_color();
    // Dummy window to for input, avoids extra refresh on wgetch call
    WINDOW* dummy = newwin(1, 1, 1000, 1000); 
    keypad(dummy, true);
    curs_set(0);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    gamew = (CARD_W + 2) * 3;
    gameh = (CARD_H + 2) * 3;
    getmaxyx(stdscr, wrows, wcols);
    if (wrows < gameh || wcols < gamew) {
      fprintf(stderr, "aborting because either %d < %d or %d < %d\n", wrows, gamew, wcols, gameh);
      return 1; 
    };
    tlcornerx = (wcols - gamew) / 2;
    tlcornery = (wrows - gameh) / 2;
    for (int i=0; i<9; i++) {
      card_windows[i] = newwin(CARD_H+2, CARD_W+2, tlcornery + (i / 3) * (CARD_H + 2), tlcornerx + (i % 3) * (CARD_W + 2));
    }


    // Draw all 27 cards in succession
    for (int j=0; j<9; j++) {
      draw_border(card_windows[j], '!');
      draw_card(card_windows[j], cards[j], dummy);
    }
    
    wgetch(dummy);
    endwin();
    return EXIT_SUCCESS;
}
