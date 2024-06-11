#ifndef HELPERS_H
#define HELPERS_H

#include "components.hpp"
#include "menu_functions.hpp"

#include <ncurses.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <menu.h>



#define MAX_LINE 1024
#define PREFIX "export ROS_MASTER_URI=http://"
#define SUFFIX ":11311"
#define BACKUP_SUFFIX ".bak"
#define FILE_PATH "/home/vscode/.bashrc"

int add_uri_function(WINDOW *win);
int delete_uri_function(WINDOW *win);
int view_uri_function(WINDOW *win);

void handle_menu_selection(const char *choice , WINDOW *func_win) {
    
    if (strcmp(choice, "Add URI") == 0) 
    {
        mvwprintw(func_win, 1, 2, "Current Panel: Add URI");
        add_uri_function(func_win);
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

bool check_ip_validity(const char *ip){
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) == 1) {
        return true;
    }
    return false;}

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

void append_to_bashrc(const char *new_line, const char *filename, WINDOW *win) {
    
    FILE *file = fopen(filename, "a"); // Open in append mode
    if (file == NULL) 
    {
        view_error_message(win, "Error opening .bashrc file. Exiting...");
        sleep(2);
        kill_the_program();
        return;
    }

    size_t total_length = strlen(PREFIX) + strlen(new_line) + strlen(SUFFIX) + 1;

    char *line = (char *)malloc(total_length * sizeof(char));
    if (line == NULL) 
    {
        view_error_message(win, "Memory allocation failed. Exiting...");
        sleep(2);
        kill_the_program();
        return;
    }

    strcpy(line, PREFIX);
    strcat(line, new_line);
    strcat(line, SUFFIX);

    fprintf(file, "%s\n", line); 
    fclose(file);
    free(line); 
    
    mvwprintw(win, 6, 2, "Line added to .bashrc successfully.");
    wrefresh(win);
}

void free_lines(char** lines, int line_count) {
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
}

void get_ip_input(WINDOW *win, char *ip_buffer, size_t buffer_size) {
    mvwprintw(win, 4, 2, "Enter IP address: ");
    wrefresh(win);

    echo();
    wgetnstr(win, ip_buffer, buffer_size - 1);
    noecho();
}

#endif