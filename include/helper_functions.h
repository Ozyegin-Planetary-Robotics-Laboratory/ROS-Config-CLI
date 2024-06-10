#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

bool check_ip_validity();
void view_error_message(WINDOW *win, const char *message);

char** read_file(const char* filename, int* line_count, WINDOW *win);
void delete_from_bash(const char* linename, const char* filename);
void add_to_bash(const char* linename, const char* filename);
void free_lines(char** lines, int line_count);
void handle_menu_selection(const char *choice, WINDOW *func_win);
void cleanup_and_exit(WINDOW *menu_win, WINDOW *func_win, MENU *menu);
void check_for_terminal_size(int rows, int cols);
void handle_menu_selection(const char *choice , WINDOW *func_win);
void cleanup_and_exit(WINDOW *menu_win, WINDOW *func_win, MENU *menu);

#endif 
