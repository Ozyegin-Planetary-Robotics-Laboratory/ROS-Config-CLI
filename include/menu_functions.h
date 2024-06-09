#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H

#include <ncurses.h>

#define PREFIX "export ROS_MASTER_URI=http://"
#define MAX_LINE 1024

char** read_file(const char* filename, int* line_count);
void free_lines(char** lines, int line_count);

int view_uri_function(WINDOW *func_win);
int delete_uri_function(WINDOW *func_win);
int add_uri_function();
int edit_uri_function(WINDOW *func_win);

#endif // MENU_FUNCTIONS_H
