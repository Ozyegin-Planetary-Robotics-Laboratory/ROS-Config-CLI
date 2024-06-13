#include <ncurses.h>
#include <menu.h>
#include <cstdlib> 
#include <cstring>
#include <cstdlib> // For getenv function

#include "../include/components.hpp"
#include "../include/helpers.hpp"
#include "../include/rosnetconfig.hpp"
#include "../include/menu_functions.hpp"
#define MENU_WIDTH 30  
#define INFO_HEIGHT 2


int main(){

    init_ncurses();

    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

    WINDOW *menu_win = create_newwin(term_height, MENU_WIDTH, 0, 0);
    draw_border(menu_win);

    WINDOW *info_win = create_newwin(INFO_HEIGHT, term_width - MENU_WIDTH, 0, MENU_WIDTH);
    draw_border(info_win);
    
    WINDOW *main_win = create_newwin(term_height - INFO_HEIGHT, term_width - MENU_WIDTH, INFO_HEIGHT, MENU_WIDTH);
    draw_border(main_win);

    const char *choices[] = {
        "View configs",
        "Add config",
        "Delete config",
        "View System Env",
        "Update System Env",
        "Exit"
    };

    MENU *menu = create_menu(choices, 6);
    set_menu_win(menu, menu_win);
    set_menu_sub(menu, derwin(menu_win, term_height/2, MENU_WIDTH -2 ,1, 1));
    set_menu_mark(menu, " * ");
    post_menu(menu);
    wrefresh(menu_win);
    
    if (!check_rosconfig_exists()) 
    {
        char *home = getenv("HOME");
        std::string home_path = home;
        view_message_at_info(info_win, ("File not found at " + home_path + ". Creating one...").c_str(), 0, 0);
        
        sleep(1);
        
        if (!create_rosconfig_file()) 
        {
            view_message_at_info(info_win, "Error creating .rosconfig file", 0, 0);
        }

        view_message_at_info(info_win, ("Created .rosconfig file at " + home_path).c_str(), 0, 0);
    }
    else 
    {
        view_message_at_info(info_win, ".rosconfig file found. You can use the program with arrow keys", 0, 0);
    }
    
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

                wclear(main_win);
                wrefresh(main_win);
                handle_menu_selection(choice, main_win, info_win); 
            }
            break;
        }
        wrefresh(menu_win);
        wrefresh(main_win);
    }

    exit:
    unpost_menu(menu);
    free_menu(menu);
    endwin();
    return 0;


}