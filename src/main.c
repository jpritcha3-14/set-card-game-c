#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#include "helper_functions.h" 
#include "structs.h"
#include "menu.h"
#include "set.h"
#include "leaderboard.h" 

#if !defined(BASEPATH)
const char ASSETPATH[] = "/usr/local/share/set-game/assets/";
#else
const char ASSETPATH[] = BASEPATH;
#endif

char DBPATH[100];

pthread_mutex_t lock;

int main() {
    WINDOW *card_windows[12];
    WINDOW *messages, *card_count, *set_count, *timer;
    char cards[27][CARD_H][CARD_W];
    char logo[3][LOGO_LETTER_H][LOGO_LETTER_W];
    int wrows, wcols, gamew, gameh, tlcornerx, tlcornery, final_time;
    srand(time(0));

    char logo_path[100];
    char card_path[100];
    strcpy(logo_path, ASSETPATH);
    strcpy(card_path, ASSETPATH);
    strcpy(DBPATH, ASSETPATH);
    strcat(logo_path, "logo.txt");
    strcat(card_path, "cards.txt");
    strcat(DBPATH, "leaderboard.db");

    load_cards(cards, card_path);
    load_logo(logo, logo_path);

    initscr();
    noecho();
    start_color();
    // Dummy window to for input, avoids extra refresh on wgetch call
    WINDOW* dummy = newwin(1, 1, 1000, 1000); 
    keypad(dummy, true);
    curs_set(0);
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);

    gamew = (CARD_W + 2) * 4;
    gameh = (CARD_H + 2) * 3;
    getmaxyx(stdscr, wrows, wcols);
    if (wrows < gameh || wcols < gamew) {
      fprintf(stderr, "aborting because either %d < %d or %d < %d\n", wrows, gamew, wcols, gameh);
      return 1; 
    };
    tlcornerx = (wcols - gamew) / 2;
    tlcornery = (wrows - gameh) / 2;

    WINDOW* logo_window = newwin(LOGO_LETTER_H, LOGO_LETTER_W*3, tlcornery, tlcornerx + LOGO_LETTER_W*3/2); 
    WINDOW* menu_window = newwin(LOGO_LETTER_H, LOGO_LETTER_W*3, tlcornery+LOGO_LETTER_H, tlcornerx + LOGO_LETTER_W*3/2); 
    WINDOW* leaderboard_window = newwin(LOGO_LETTER_H*2, LOGO_LETTER_W*3, tlcornery, tlcornerx + LOGO_LETTER_W*3/2); 
    Option menu_selection;
    //fprintf(stderr, "%d %d\n", tlcornerx, tlcornery);
    messages = newwin(1, MESSAGE_W, tlcornery-1, tlcornerx);
    card_count = newwin(1, SET_COUNT_W, tlcornery+3*(CARD_H+2), tlcornerx);
    set_count = newwin(1, SET_COUNT_W, tlcornery+3*(CARD_H+2), tlcornerx + 3*(CARD_W+2));
    timer = newwin(1, SET_COUNT_W, tlcornery-1, tlcornerx + 3*(CARD_W+2));
    wattron(messages, COLOR_PAIR(WHITE));
    wattron(set_count, COLOR_PAIR(WHITE));
    wattron(timer, COLOR_PAIR(WHITE));

    for (int i=0; i<12; i++) {
      card_windows[i] = newwin(CARD_H+2, CARD_W+2, tlcornery + (i / 4) * (CARD_H + 2), tlcornerx + (i % 4) * (CARD_W + 2));
    }
   
    for (;;) {
      draw_logo(logo_window, logo);
      menu_selection = show_menu(menu_window);
      clear_screen();
      if (menu_selection == start) {
        final_time = play_game(card_windows, messages, card_count, set_count, timer, cards, dummy);
        if (final_time < INT_MAX) add_leaderboard_time(leaderboard_window, final_time);
      } else if (menu_selection == leaderboard) {
        show_leaderboard(leaderboard_window, dummy); 
      } else if (menu_selection == quit) break;
    }
    endwin();
    return EXIT_SUCCESS;
}
