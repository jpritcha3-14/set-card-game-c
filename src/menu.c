#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include "structs.h"

const char* CURSOR_START = ">>>";
const char* CURSOR_END = "<<<";

const char *menu_option_text[] = {
  "Start",
  "How To Play",
  "Leaderboard",
  "Quit"
};

void draw_menu(WINDOW *menu_window) {
  wattron(menu_window, COLOR_PAIR(WHITE));
  int num_options = (int)(sizeof(menu_option_text)/sizeof(menu_option_text[0]));
  int start;
  for (int i=0; i<num_options; i++) { 
    start = LOGO_LETTER_W*3/2 - strlen(menu_option_text[i])/2;
    mvwaddstr(menu_window, 2*i+1, start, menu_option_text[i]);
  }
}

void draw_cursor(WINDOW *menu_window, int old, int new) {  
  int old_start = LOGO_LETTER_W*3/2 - strlen(menu_option_text[old])/2;
  int old_end = old_start + strlen(menu_option_text[old]);
  int new_start = LOGO_LETTER_W*3/2 - strlen(menu_option_text[new])/2;
  int new_end = new_start + strlen(menu_option_text[new]);
  wattron(menu_window, COLOR_PAIR(WHITE));
  mvwaddstr(menu_window, 2*old+1, old_start, menu_option_text[old]);
  for (int i=old_start-6; i<old_start-1; i++) {
    mvwaddch(menu_window, 2*old+1, i, ' ');
  }
  for (int i=old_end+1; i<old_end+6; i++) {
    mvwaddch(menu_window, 2*old+1, i, ' ');
  }
  wattron(menu_window, COLOR_PAIR(new % 3 + 1));
  mvwaddstr(menu_window, 2*new+1, new_start, menu_option_text[new]);
  mvwaddstr(menu_window, 2*new+1, new_start - strlen(CURSOR_START) - 1, CURSOR_START);
  mvwaddstr(menu_window, 2*new+1, new_end + 1, CURSOR_END);
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
