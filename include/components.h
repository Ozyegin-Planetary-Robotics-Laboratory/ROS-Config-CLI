#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <ncurses.h>
#include <menu.h>

#define MENU_WIDTH 20

void init_ncurses();
WINDOW* create_window(int height, int width, int starty, int startx);
MENU* create_menu(WINDOW *win, const char *choices[], int n_choices);
void cleanup_menu(MENU *menu);
void draw_border(WINDOW *win);

#endif // COMPONENTS_H
