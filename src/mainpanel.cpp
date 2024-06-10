#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ncurses.h>


#include "components.h"
#include "menu_functions.h"
#include "helper_functions.h"



int main(){

    init_ncurses();

    const char *choices[] = {
        "View URI",
        "Add URI",
        "Delete URI",
        "Exit"
    };

    int n_choices = sizeof(choices) / sizeof(choices[0]);
    int term_h, term_w;

    getmaxyx(stdscr, term_h, term_w);

    check_for_terminal_size(term_h, term_w);

    WINDOW *menu_win = create_window(term_h - 2 , 40, 1, 1);
    draw_border(menu_win);
    refresh();

    WINDOW *func_win = create_window(term_h - 2, term_w - 44, 1, 42);
    draw_border(func_win); // Draw border for func_win
    refresh();

    MENU *menu = create_menu(menu_win, choices, n_choices);
    refresh();

    int ch;
    while((ch = getch()) != 'q')
    { 
        switch(ch)
        {
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
                
                if (strcmp(choice, "Exit") == 0) 
                {
                    goto exit;
                }

                wclear(func_win);
                wrefresh(func_win);
                handle_menu_selection(choice, func_win); 
            }
            break;
        }
        wrefresh(menu_win);
        wrefresh(func_win);
    }
    exit:
    cleanup_and_exit(menu_win, func_win, menu);
    return 0;
}