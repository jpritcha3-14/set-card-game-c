#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include "structs.h"

const char* CURSOR = "-->";

const char *menu_option_text[] = {
  "Start",
  "How To Play",
  "Highscores",
  "Quit"
};

void draw_menu(WINDOW *menu_window) {
  int num_options = (int)(sizeof(menu_option_text)/sizeof(menu_option_text[0]));
  for (int i=0; i<num_options; i++) { 
    mvwaddstr(menu_window, 2*i+1, LOGO_LETTER_W*3/2, menu_option_text[i]);
  }
}

void draw_cursor(WINDOW *menu_window, int old, int new) {  
  for (int i=0; i<LOGO_LETTER_W*3/2 - 1; i++) {
    mvwaddch(menu_window, 2*old+1, i, ' ');
  }
    mvwaddstr(menu_window, 2*new+1, LOGO_LETTER_W*3/2 - strlen(CURSOR) - 1, CURSOR);
}

Option show_menu(WINDOW *menu_window) {
  Option cur_option = start;
  Option prev_option = start;
  char action;

  draw_menu(menu_window);
  draw_cursor(menu_window, prev_option, cur_option);
  wrefresh(menu_window);
  action = wgetch(menu_window);
  while (action != ' ') {
    if (action == 'w' && cur_option != start) {
      prev_option = cur_option--;
    } else if (action == 's' && cur_option != quit) {
      prev_option = cur_option++;
    } else {
      action = wgetch(menu_window);
      continue;
    }
    draw_cursor(menu_window, prev_option, cur_option);
    wrefresh(menu_window);
    action = wgetch(menu_window);
  } 
  return cur_option;
}
