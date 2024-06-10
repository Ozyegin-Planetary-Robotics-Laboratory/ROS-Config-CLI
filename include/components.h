#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>

void init_ncurses();
WINDOW* create_window(int height, int width, int starty, int startx);
void draw_border(WINDOW* win);
MENU* create_menu(WINDOW* menu_win, const char** choices, int n_choices);

#endif // COMPONENTS_H
