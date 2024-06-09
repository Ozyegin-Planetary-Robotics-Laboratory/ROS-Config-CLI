#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ncurses.h>

#include "../include/components.h"
#include "../include/menu_functions.h" 

void handle_menu_selection(const char *choice, WINDOW *func_win);
void cleanup_and_exit(WINDOW *menu_win, WINDOW *func_win, MENU *menu);

int main(){

    init_ncurses();
    const char *choices[] = {
        "View URI",
        "Add URI",
        "Edit URI",
        "Delete URI",
        "Exit"
    };
    int n_choices = sizeof(choices) / sizeof(choices[0]);
    int term_h, term_w;

    getmaxyx(stdscr, term_h, term_w);

    if (term_h < 4 || term_w < 42) {
        endwin();
        fprintf(stderr, "Your terminal window is too small. Minimum size required: 4x42\n");
        return 1;
    }

    WINDOW *menu_win = create_window(term_h - 2 , 40, 1, 1);
    draw_border(menu_win); // Draw border for menu_win
    refresh();

    WINDOW *func_win = create_window(term_h - 2, term_w - 42, 1, 42);
    draw_border(func_win); // Draw border for func_win
    refresh();

    MENU *menu = create_menu(menu_win, choices, n_choices);
    refresh();

    int ch;
    while((ch = getch()) != 'q'){ // exit when F1 key is pressed
        switch(ch){
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;

            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;

            case 10: 
            {
                ITEM *cur = current_item(menu);
                const char *choice = item_name(cur);
                if (strcmp(choice, "Exit") == 0) {
                    goto exit;
                }
                handle_menu_selection(choice, func_win); 
            }
            break;
        }
        wrefresh(menu_win);
        wrefresh(func_win);
    }

    exit:
    unpost_menu(menu);
    free_menu(menu);
    delwin(menu_win);
    endwin();

    return 0;
}

void handle_menu_selection(const char *choice , WINDOW *func_win) {
    if (strcmp(choice, "Add URI") == 0) {
        wclear(func_win);
        wrefresh(func_win);
        mvwprintw(func_win, 1, 2, "Current Panel: Add URI");
        add_uri_function();
        draw_border(func_win);

    } else if (strcmp(choice, "Edit URI") == 0) {
        wclear(func_win);
        wrefresh(func_win);
        mvwprintw(func_win, 1, 2, "Current Panel: Edit URI");
        edit_uri_function(func_win);
        draw_border(func_win);

    } else if (strcmp(choice, "Delete URI") == 0) {
        wclear(func_win);
        wrefresh(func_win);
        mvwprintw(func_win, 1,2, "Current Panel: Delete URI");
        delete_uri_function(func_win);
        draw_border(func_win);

    } else if (strcmp(choice, "View URI") == 0) {
        wclear(func_win);
        wrefresh(func_win);
        mvwprintw(func_win, 1, 2, "Current Panel: View URI");
        view_uri_function(func_win);
        draw_border(func_win);

    }
}

void cleanup_and_exit(WINDOW *menu_win, WINDOW *func_win, MENU *menu) {
    unpost_menu(menu);
    free_menu(menu);

    delwin(menu_win);
    delwin(func_win);

    endwin();
}