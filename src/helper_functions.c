#include <stdlib.h>
#include <ncurses.h>

#include "structs.h"

wloc* get_game_area(WINDOW* w) {
    wloc* game_area = malloc(sizeof(wloc)); 
    if (game_area == NULL) {
        endwin(); 
        printf("Malloc failed (logo)\n");
        exit(1);
    }
    int wrows, wcols;
    getmaxyx(w, wrows, wcols);
    // Terminal too small for game
    if (wrows < 22 || wcols < 60) {
        free(game_area);
        return NULL;
    }
    game_area->x = (wcols - 60) / 2;
    game_area->y = (wrows - 22) / 2;
    game_area->rows = 11;
    game_area->cols = 60;
    return game_area;
}
