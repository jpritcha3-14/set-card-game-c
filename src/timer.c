#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <ncurses.h>
#include <pthread.h>

#include "structs.h"

extern int signal_done;
extern int seconds_elapsed;
extern pthread_mutex_t lock;

void *thread_timer_function(void *timer_window) {
  WINDOW *my_window = (WINDOW *)timer_window;
  char display_time[20];
  struct timespec wait;
  wait.tv_sec = 0;
  wait.tv_nsec = 50000000L;
  time_t start_time, cur_time;

  seconds_elapsed = 0;
  int diff = 0;
  nanosleep(&wait, NULL); //wait for initial screen draw

  pthread_mutex_lock(&lock);
  mvwaddstr(my_window, 0, 0, "00:00");
  wrefresh(my_window);
  pthread_mutex_unlock(&lock);

  time(&start_time);

  for (;;) {
    pthread_mutex_lock(&lock);
    if (signal_done == 1) {
      signal_done = 0;
      pthread_mutex_unlock(&lock);
      break;
    }
    pthread_mutex_unlock(&lock);

    time(&cur_time);
    diff = (int)difftime(cur_time, start_time);

    if (diff > seconds_elapsed) {
      seconds_elapsed = diff; 
      snprintf(display_time, 20, "%02d:%02d", seconds_elapsed / 60, seconds_elapsed % 60);
      pthread_mutex_lock(&lock);
      mvwaddstr(my_window, 0, 0, display_time);
      wrefresh(my_window);
      pthread_mutex_unlock(&lock);
    }

    nanosleep(&wait, NULL);
  }

  return NULL;
}
