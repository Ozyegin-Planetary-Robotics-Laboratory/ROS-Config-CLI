#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include <string.h>
#include <stdlib.h>
#include <cctype>  
#include "helper_functions.h"

#define PREFIX "export ROS_MASTER_URI=http://"
#define FILE_PATH "/home/vscode/.bashrc"
#define MAX_LINE 1024
#define BACKUP_SUFFIX ".bak"
#define ERORR_LINE_X 40
#define ERROR_LINE_Y 1

int view_uri_function(WINDOW *win) {
    int line_count;
    char** lines = read_file(FILE_PATH, &line_count,win);
    if (line_count == 0) 
    {
        view_error_message(win, "No URI found");
    }
    else
    {
        for (int i = 0; i < line_count; i++) 
        {
            mvwprintw(win, i + 4, 2, "%d. %s", i + 1, lines[i]);
        }
    }
    free_lines(lines, line_count);
    return 0;
}

int delete_uri_function(WINDOW *win) {
    int line_count;
    char** lines = read_file(FILE_PATH, &line_count,win);
    if (line_count == 0) 
    {
        view_error_message(win, "No URI found");
    } 
    else 
    {
        mvwprintw(win, 3, 2, "Select the URI to delete:");

        for (int i = 0; i < line_count; i++) 
        {
            mvwprintw(win, i + 4, 2, "%d. %s", i + 1, lines[i]);
        }
        
        mvwprintw(win, line_count + 6, 2, "Enter the number of the URI you want to delete: ");
        wrefresh(win);

        char input[10] = {0};
        int pos = 0;
        int ch;

        while ((ch = wgetch(win)) != '\n' && pos < 9) 
        {
            if (ch == KEY_BACKSPACE || ch == 127) 
            {
                if (pos > 0) 
                { 
                    --pos;
                    input[pos] = '\0';
                    mvwprintw(win, line_count + 7, 4 + pos, " ");
                    wmove(win, line_count + 7, 4 + pos); 
                    wrefresh(win);
                }
            } 
            else if (isdigit(ch)) 
            {
                input[pos++] = ch;
                mvwprintw(win, line_count + 7, 2, "%c", ch); 
                wrefresh(win);
            }
        }

        input[pos] = '\0';  
        int choice = atoi(input) - 1;

        if (choice >= 0 && choice < line_count) 
        {
            mvwprintw(win, line_count + 8, 2, "Deleting URI: %s", lines[choice]);
            delete_from_bash(lines[choice], FILE_PATH);
        } 
        else 
        {
            mvwprintw(win, line_count + 8, 2, "Invalid choice");
        }
        wrefresh(win);       
    }
    return 0;
}

int add_uri_function(){
    return 0;
}

