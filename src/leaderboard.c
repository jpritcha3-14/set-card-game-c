#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <sqlite3.h>

#include "leaderboard.h"
#include "helper_functions.h"

extern const char DBPATH[];

struct rowtime {
    int row;
    int time;
};

void check_db(int rc, sqlite3 *db);
void create_db(const char* filename);
int get_num_entries(const char* table);
void append_leaderboard_time(const char* table, const char* name, const int time);
void update_leaderboard_time(const char* table, const int highrow, const char* name, const int time);
struct rowtime* get_max_row_and_time(const char* table);

void check_db(int rc, sqlite3 *db) {
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}
}

void create_db(const char* filename) {
    // Open db connection
    sqlite3 *db;
    int rc = sqlite3_open(DBPATH, &db);
    check_db(rc, db);
    sqlite3_close(db);

}

void show_leaderboard(WINDOW* lbw, WINDOW* dummy) {
    wattron(lbw, A_BOLD);
    wattron(lbw, COLOR_PAIR(1));
    mvwaddstr(lbw, 0, 0, "Leaderboard");

    wattron(lbw, COLOR_PAIR(2));
    mvwaddstr(lbw, 2, 3, "Name");
    mvwaddstr(lbw, 2, 15, "Time");

    sqlite3 *db;
    sqlite3_stmt *res;
    sqlite3_stmt *table_res;
    int row = 3;
    char table[] = "times";

    // Open db connection
    int rc = sqlite3_open(DBPATH, &db);
    check_db(rc, db);
    
    // Create table if it doesn't exit 
    char table_sql[128];
    sprintf(table_sql, "CREATE TABLE IF NOT EXISTS %s(name TEXT, time INTEGER);", table);
    rc = sqlite3_prepare_v2(db, table_sql, -1, &table_res, 0);
    sqlite3_step(table_res);
    sqlite3_finalize(table_res);

    // Select all scores from highest to lowest
    char sql[128]; 
    sprintf(sql, "SELECT name, time FROM %s ORDER BY time ASC;", table);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute select: %s\n", sqlite3_errmsg(db)); 
        return;
    }

    // Print all returned rows
    char buff[10];
    while(sqlite3_step(res) == SQLITE_ROW) {
        wattron(lbw, COLOR_PAIR((row + 1) % 2 + 1));
        strcpy(buff, "");
        sprintf(buff, "%d", sqlite3_column_int(res, 1));
        mvwaddstr(lbw, row, 3, (const char*)sqlite3_column_text(res, 0));
        mvwaddstr(lbw, row, 15, buff);
        row += 1;
    }
    
    sqlite3_finalize(res);
    sqlite3_close(db);

    wrefresh(lbw); // Show printed scores 
    wgetch(dummy); // Wait for user input to return to menu
    clear_screen();
    return;
}

void add_leaderboard_time(WINDOW* lbw, int time) {
    char table[] = "times";
    wattron(lbw, A_BOLD);
    int entries = get_num_entries(table);
    struct rowtime* rt = get_max_row_and_time(table);

    // Allow user to enter name
    if (entries < 10 || time < rt->time) {
        echo(); // Show input
        curs_set(2); // Show cursor
        char lb_msg[32];
        sprintf(lb_msg, "New Leaderboard Time: %02d:%02d:%02d", time / 3600, time / 60, time % 60);
        char prompt[] = "Enter your name";
        char name[11];
        wattron(lbw, COLOR_PAIR(1));
        mvwprintw(lbw, 0, 0, lb_msg);
        wattron(lbw, COLOR_PAIR(2));
        mvwprintw(lbw, 2, 0, prompt);
        wattroff(lbw, COLOR_PAIR);
        wattron(lbw, A_BOLD);
        wmove(lbw, 3, 0);
        wgetnstr(lbw, name, 10); // Put input into name
        if (entries < 10) {
            append_leaderboard_time(table, name, time);
        } else {
            update_leaderboard_time(table, rt->row, name, time);
        }
        curs_set(0);
        noecho();
    }
    free(rt);
}

int get_num_entries(const char* table) {
    sqlite3 *db;
    sqlite3_stmt *table_res;
    sqlite3_stmt *count_res;

    // Open db connection
    int rc = sqlite3_open(DBPATH, &db);
    check_db(rc, db);

    // Create table if it doesn't exit 
    char table_sql[128];
    sprintf(table_sql, "CREATE TABLE IF NOT EXISTS %s(name TEXT, time INTEGER);", table);
    rc = sqlite3_prepare_v2(db, table_sql, -1, &table_res, 0);
    sqlite3_step(table_res);
    sqlite3_finalize(table_res);

    // Get number of values in table 
    char* count_sql = malloc(sizeof(char)*1024);
    int entry_count = 0;
    sprintf(count_sql, "SELECT COUNT(*) FROM %s;", table);
    rc = sqlite3_prepare_v2(db, count_sql, -1, &count_res, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute count prepare: %s\n", sqlite3_errmsg(db)); 
        return 1;
    }
    rc = sqlite3_step(count_res);
    if (rc != SQLITE_ROW) {
        fprintf(stderr, "Failed to execute count step: %s\n", sqlite3_errmsg(db)); 
        return 1;
    }
    entry_count = sqlite3_column_int(count_res, 0); 
    sqlite3_finalize(count_res);
    return entry_count;
}

void append_leaderboard_time(const char* table, const char* name, const int time) {

    sqlite3 *db;
    sqlite3_stmt *res;
    
    // Open db connection
    int rc = sqlite3_open(DBPATH, &db);
    if (rc != SQLITE_OK) exit(1);
    check_db(rc, db);
    
    // Insert values into table
    char sql[128];
    sprintf(sql, "INSERT INTO %s(name, time) VALUES(?, ?);", table);

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
        if (sqlite3_bind_text(res, 1, name, strlen(name), SQLITE_STATIC) != SQLITE_OK) {
            fprintf(stderr, "Problem with text bind: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(res);
            sqlite3_close(db);
            return;
        }
        if (sqlite3_bind_int(res, 2, time) != SQLITE_OK) {
            fprintf(stderr, "Problem with int bind: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(res);
            sqlite3_close(db);
            return;
        }
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db)); 
        sqlite3_finalize(res);
        sqlite3_close(db);
        return;
    }

    sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);
}


void update_leaderboard_time(const char* table, const int highrow, const char* name, const int time) {
    sqlite3 *db;
    sqlite3_stmt *res;
    
    // Open db connection
    int rc = sqlite3_open(DBPATH, &db);
    check_db(rc, db);

    // Update lowest value in table 
    char sql[128];
    sprintf(sql, "UPDATE %s SET name=?, time=? WHERE rowid=?;", table);

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
        if (sqlite3_bind_text(res, 1, name, strlen(name), SQLITE_STATIC) != SQLITE_OK) {
            fprintf(stderr, "Problem with text bind: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(res);
            sqlite3_close(db);
            return;
        }
        if (sqlite3_bind_int(res, 2, time) != SQLITE_OK) {
            fprintf(stderr, "Problem with int bind: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(res);
            sqlite3_close(db);
            return;
        }
        if (sqlite3_bind_int(res, 3, highrow) != SQLITE_OK) {
            fprintf(stderr, "Problem with int bind: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(res);
            sqlite3_close(db);
            return;
        }
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db)); 
        sqlite3_finalize(res);
        sqlite3_close(db);
        return;
    }

    sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);
}

struct rowtime* get_max_row_and_time(const char* table) {
    sqlite3 *db;
    sqlite3_stmt *res;
    
    // Open db connection
    int rc = sqlite3_open(DBPATH, &db);
    check_db(rc, db);

    // Get lowest score and its rowid
    char sql[128];
    sprintf(sql, "SELECT rowid, time FROM %s ORDER BY time DESC LIMIT 1", table);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    struct rowtime* rt = malloc(sizeof(struct rowtime));
    
    sqlite3_step(res);
    rt->row = sqlite3_column_int(res, 0);
    rt->time = sqlite3_column_int(res, 1);
    sqlite3_finalize(res);
    sqlite3_close(db);
    return rt;
}
