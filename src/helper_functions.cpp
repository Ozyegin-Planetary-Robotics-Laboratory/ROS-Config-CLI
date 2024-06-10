#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include <string.h>
#include <stdlib.h>
#include <cctype>  
#include "../include/menu_functions.h"
#include "../include/helper_functions.h"
#include "../include/components.h"
#include <unistd.h>

#define MAX_LINE 1024
#define PREFIX "export ROS_MASTER_URI=http://"
#define BACKUP_SUFFIX ".bak"
#define ERORR_LINE_X 40
#define ERROR_LINE_Y 1
#define FILE_PATH "/home/vscode/.bashrc"


bool check_ip_validity(){
    return true;
}

void view_error_message(WINDOW *win, const char *message){

    int rows, cols;
    getmaxyx(win, rows, cols);

    for (int i = rows - 3; i < rows; ++i)
    {
        mvwprintw(win, i, 0, "%*s", cols, ""); 
    }

    mvwprintw(win, rows - 2, 2, "%s", message);
    wrefresh(win);
}

void kill_the_program(){
    cleanup_and_exit(NULL, NULL, NULL);
    exit(EXIT_FAILURE);
}

void check_for_terminal_size(int rows, int cols){
    
    if(rows < 20 || cols < 60)
    {
        endwin();
        printf("Current window size: %dx%d\n", cols, rows);
        fprintf(stderr, "Terminal size must be at least 80x20\n");
        exit(EXIT_FAILURE);
    }
    return;
}

void handle_menu_selection(const char *choice , WINDOW *func_win) {
    
    if (strcmp(choice, "Add URI") == 0) 
    {
        mvwprintw(func_win, 1, 2, "Current Panel: Add URI");
        add_uri_function();
        draw_border(func_win);
    }  
    else if (strcmp(choice, "Delete URI") == 0)
    {
        mvwprintw(func_win, 1,2, "Current Panel: Delete URI");
        delete_uri_function(func_win);
        draw_border(func_win);

    } 
    else if (strcmp(choice, "View URI") == 0)
    {
        mvwprintw(func_win, 1, 2, "Current Panel: View URI");
        view_uri_function(func_win);
        draw_border(func_win);
    }
}

void cleanup_and_exit(WINDOW *menu_win, WINDOW *func_win, MENU *menu) {
    if (menu != NULL) {
        unpost_menu(menu);
        free_menu(menu);
    }
    
    if (menu_win != NULL) {
        delwin(menu_win);
    }

    if (func_win != NULL) {
        delwin(func_win);
    }
    endwin();
}

char** read_file(const char* filename, int* line_count, WINDOW *win) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) 
    {
        view_error_message(win, "Error opening file. Exiting...");
        sleep(2);
        kill_the_program();
        return NULL;
    }

    char** lines = NULL;
    char line[MAX_LINE];
    int count = 0;
    
    while (fgets(line, MAX_LINE, file) != NULL) 
    {
        if (strncmp(line, PREFIX, strlen(PREFIX)) == 0) 
        {
            char* new_line = (char *)malloc((strlen(line) + 1) * sizeof(char));
            if (new_line == NULL) 
            {
                view_error_message(stdscr, "Memory allocation error. Exiting...");
                sleep(2);
                kill_the_program();
                fclose(file);
                return NULL;
            }
            
            strcpy(new_line, line);
            char** temp = (char **)realloc(lines, (count + 1) * sizeof(char *));
            
            if (temp == NULL) 
            {
                view_error_message(stdscr, "Memory allocation error. Exiting...");
                sleep(2);
                kill_the_program();
                fclose(file);
                free(new_line);
                return NULL;
            }
            lines = temp;

            lines[count] = new_line;
            count++;
        }
    }
    fclose(file);
    *line_count = count;
    return lines;
}

void delete_from_bash(const char *linename, const char *filename) {
    char backup_filename[MAX_LINE];
    snprintf(backup_filename, sizeof(backup_filename), "%s%s", filename, BACKUP_SUFFIX);

    if (rename(filename, backup_filename) != 0) 
    {
        view_error_message(stdscr, "Error creating backup file. Exiting...");
        sleep(2);
        kill_the_program();
        return;
    }

    FILE *file = fopen(backup_filename, "r");
    if (file == NULL) 
    {
        view_error_message(stdscr, "Error opening backup file. Exiting...");
        sleep(2);
        kill_the_program();
        return;
    }

    FILE *temp = fopen(filename, "w");
    if (temp == NULL) 
    {
        view_error_message(stdscr, "Error opening file. Exiting...");
        sleep(2);
        kill_the_program();
        fclose(file);
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file) != NULL) 
    {
        if (strncmp(line, linename, strlen(linename)) != 0) 
        {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);
}

void add_to_bash(const char *line, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }

    fputs(line, file);
    fclose(file);
}

void free_lines(char** lines, int line_count) {
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
}