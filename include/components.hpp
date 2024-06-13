#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <ncurses.h>
#include <menu.h>
#include <cstdlib> // for calloc

void init_ncurses(){
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

WINDOW *create_newwin(int height, int width, int starty, int startx){
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}

MENU *create_menu(const char *choices[], int n_choices){
    ITEM **my_items;
    MENU *my_menu;
    my_items = (ITEM **) calloc(n_choices + 1, sizeof(ITEM *));
    for(int i = 0; i < n_choices; i++){
        my_items[i] = new_item(choices[i], "");
    }
    my_items[n_choices] = (ITEM *)NULL;
    my_menu = new_menu((ITEM **)my_items);
    return my_menu;
}


#endif