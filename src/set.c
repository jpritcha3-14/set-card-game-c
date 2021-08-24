#include <stdio.h> 
#include <pthread.h>

#include "helper_functions.h"
#include "timer.h"
#include "structs.h"

int signal_done = 0;
pthread_mutex_t lock;

int play_game(WINDOW *card_windows[], WINDOW* messages, WINDOW* card_count, WINDOW* set_count, WINDOW *timer_window, char cards[][CARD_H][CARD_W], WINDOW *dummy) {
  pthread_t thread_id;
  int selected[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int max_card = 81; 
  //int max_card = 24; 
  int deck[81];
  card_props props[81];
  int cur_card = 0;
  int prev_card = 0;
  char inp;
  int sets_on_board;
  char set_count_message[3];
  char card_count_message[3];

  // Initialize deck and props
  for (int i=0; i<81; i++) {
    deck[i] = i; 
    props[i].color = get_color(i); 
    props[i].number = get_number(i); 
    props[i].shade = get_shade(i); 
    props[i].shape = get_shape(i); 
    //fprintf(stderr, "%d %d %d %d\n", props[i].color, props[i].number, props[i].shade, props[i].shape);
  }

  shuffle(deck, 81);

  // Draw first 12 cards
  for (int i=0; i<12; i++) {  
    draw_card(card_windows[i], cards[deck[i] % 27], props[deck[i]].color);
  }

  // draw cursor start
  wattron(card_windows[cur_card], COLOR_PAIR(WHITE));
  draw_border(card_windows[cur_card], '@', '@');
  wrefresh(card_windows[cur_card]);
  mvwaddstr(set_count, 0, 0, "SETS ON BOARD: ");
  mvwaddstr(card_count, 0, 0, "CARDS IN DECK: ");
  wrefresh(set_count);
  wrefresh(card_count);

  // start timer
  pthread_create(&thread_id, NULL, thread_timer_function, timer_window);

  for (;;) {
    sets_on_board = get_set_count(min(12, max_card), deck, props);
    sprintf(set_count_message, "%2d", sets_on_board);
    sprintf(card_count_message, "%2d", max(max_card - 12, 0));
    pthread_mutex_lock(&lock);
    mvwaddstr(set_count, 0, SET_COUNT_W - 6, set_count_message);
    wrefresh(set_count);
    mvwaddstr(card_count, 0, SET_COUNT_W - 6, card_count_message);
    wrefresh(card_count);
    pthread_mutex_unlock(&lock);

    if (sets_on_board == 0) {
      if (max_card <= 21 && !any_set(max_card, deck, props)) {
        clear_message(messages, MESSAGE_W);
        mvwaddstr(messages, 0, 0, "GAME OVER, NO MORE SETS");
        wrefresh(messages);
        wgetch(dummy);
        return 0;
      }
      mvwaddstr(messages, 0, 0, "SHUFFLING...");
      wrefresh(messages);
      wgetch(dummy);
      shuffle(deck, max_card);
      for (int i=0; i<12; i++) {
        draw_card(card_windows[i], cards[deck[i] % 27], props[deck[i]].color);
      }
      continue;
    }
    clear_message(messages, MESSAGE_W);

    inp = wgetch(dummy);
    if (inp == 'q') break;
    if (inp == ' ') {
      if (cur_card >= min(max_card, 12)) continue; // Invalid selection
      select_card(card_windows[cur_card], selected, cur_card);
      if (sum(selected, 12) == 3) {
        int candidates[3];
        get_selected_cards(candidates, selected);
        if (check_set(candidates[0], candidates[1], candidates[2], deck, props) == 1)  {
          // There is a set
          mvwaddstr(messages, 0, 0, "THAT'S A SET!");
          wrefresh(messages);
          wgetch(dummy);
          max_card -= 3;
          if (max_card >= 12) {
            // Just swap in 3 new cards
            for (int i=0; i<3; i++) {
              int card_loc = candidates[i];
              swap(max_card + i, card_loc, deck); 
              int card_num = deck[card_loc];
              draw_card(card_windows[card_loc], cards[card_num % 27], props[card_num].color);
              selected[card_loc] = 0;
              draw_border(card_windows[card_loc], ' ', ' ');
            }
          } else {
            // Have to deal with fewer than 12 cards left
            for (int i=0; i<3; i++) {
              if (in_set(max_card + i, candidates)) {
                draw_blank_card(card_windows[max_card + i]);
                selected[max_card + i] = 0;
                draw_border(card_windows[max_card + i], ' ', ' ');
                //wgetch(dummy);
                continue; //card to be removed is part of current set, can be skipped
              }
              int card_loc;
              for (int j=0; j<3; j++) {
                if (candidates[j] < max_card) {
                  swap(max_card + i, candidates[j], deck); 
                  card_loc = candidates[j];
                  candidates[j] = 99;
                  break;
                }
              }
              int card_num = deck[card_loc];
              draw_card(card_windows[card_loc], cards[card_num % 27], props[card_num].color);
              //wgetch(dummy);
              draw_blank_card(card_windows[max_card + i]);
              selected[card_loc] = 0;
              draw_border(card_windows[card_loc], ' ', ' ');
              //wgetch(dummy);
            } 
          }
          wattron(card_windows[cur_card], COLOR_PAIR(WHITE));
          draw_border(card_windows[cur_card], '@', '@');
          clear_message(messages, MESSAGE_W);
        } else {
          mvwaddstr(messages, 0, 0, "NOT A SET");
          wrefresh(messages);
          wgetch(dummy);
          for (int i=0; i<3; i++) {
            int card_loc = candidates[i];
            selected[card_loc] = 0;
            draw_border(card_windows[card_loc], ' ', ' ');
          }
          wattron(card_windows[cur_card], COLOR_PAIR(WHITE));
          draw_border(card_windows[cur_card], '@', '@');
          clear_message(messages, MESSAGE_W);
        }
      }
    } else {
      prev_card = cur_card;
      cur_card = move_cursor(card_windows, selected, inp, prev_card);
    }
  }
  
  // stop timer
  pthread_mutex_lock(&lock);
  signal_done = 1;
  pthread_mutex_unlock(&lock);
  pthread_join(thread_id, NULL);

  return 0;
}
