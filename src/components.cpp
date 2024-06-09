#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#define MENU_WIDTH 20

void init_ncurses() {
    initscr();            // Initialize the window
    cbreak();             // Line buffering disabled, Pass on everything
    keypad(stdscr, TRUE); // Enable F1, F2, arrow keys, etc.
    noecho();             // Don't echo() while we do getch
    curs_set(0);          // Hide the cursor
    start_color();        // Start color functionality
}

WINDOW* create_window(int height, int width, int starty, int startx) {
    WINDOW *win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

MENU* create_menu(WINDOW *win, const char *choices[], int n_choices) {
    ITEM **items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for (int i = 0; i < n_choices; ++i) {
        items[i] = new_item(choices[i], "");
    }
    items[n_choices] = (ITEM *)NULL;

    int startx,starty;
    getmaxyx(win,starty,startx);

    MENU *menu = new_menu((ITEM **)items);
    set_menu_win(menu, win);
    set_menu_sub(menu, derwin(win, n_choices + 2, 18, (starty/2) - 2, 10)); // argumenst: height, width, starty, startx
    set_menu_mark(menu, " * ");
    post_menu(menu);
    wrefresh(win);

    return menu;
}

void cleanup_menu(MENU *menu) {
    ITEM **items = menu_items(menu);
    for (int i = 0; items[i]; ++i) {
        free_item(items[i]);
    }
    free_menu(menu);
}

void draw_border(WINDOW *win){

    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
}
