#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>
#include <menu.h>

#define PREFIX "export ROS_MASTER_URI=http://"
#define MAX_LINE 1024

char** read_file(const char* filename, int* line_count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return NULL;
    }

    char** lines = NULL;
    char line[MAX_LINE];
    int count = 0;

    // Read file line by line
    while (fgets(line, MAX_LINE, file) != NULL) {
        // Check if the line starts with the desired prefix
        if (strncmp(line, PREFIX, strlen(PREFIX)) == 0) {
            // Allocate memory for a new line and copy the content
            char* new_line = (char *)malloc((strlen(line) + 1) * sizeof(char));
            if (new_line == NULL) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(file);
                return NULL;
            }
            strcpy(new_line, line);

            // Increase the size of the lines list
            char** temp = (char **)realloc(lines, (count + 1) * sizeof(char *));
            if (temp == NULL) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(file);
                free(new_line); // Free the memory allocated for new_line
                return NULL;
            }
            lines = temp;

            // Add the new line to the list
            lines[count] = new_line;
            count++;
        }
    }

    fclose(file);

    *line_count = count;
    return lines;
}

void free_lines(char** lines, int line_count) {
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
}

int view_uri_function(WINDOW *func_win) {
    int line_count;
    char** lines = read_file("/home/vscode/.bashrc", &line_count);
    if (lines == NULL) {
        mvwprintw(func_win, 1, 2, "Error reading file");
        return 1;
    }

    if (line_count == 0) {
        mvwprintw(func_win, 2, 2, "No URI found");
    } else {
        for (int i = 0; i < line_count; i++) {
            mvwprintw(func_win, i + 4, 2, "%d. %s", i + 1, lines[i]);
        }
    }

    free_lines(lines, line_count);
    return 0;
}

int delete_uri_function(WINDOW *func_win) {
    int line_count;
    char** lines = read_file("/home/vscode/.bashrc", &line_count);
    if (lines == NULL) {
        mvwprintw(func_win, 1, 2, "Error reading file");
        return 1;
    }

    if (line_count == 0) {
        mvwprintw(func_win, 2, 2, "No URI found");
    } else {
        mvwprintw(func_win, 3, 2, "Select the URI to delete:");
        for (int i = 0; i < line_count; i++) {
            mvwprintw(func_win, i + 4, 2, "%d. %s", i + 1, lines[i]);
        }

    }
    return 0;
    
}

int add_uri_function(){
    return 0;
}

int edit_uri_function(WINDOW *func_win){
    int line_count;
    char** lines = read_file("/home/vscode/.bashrc", &line_count);
    if (lines == NULL) {
        mvwprintw(func_win, 1, 2, "Error reading file");
        return 1;
    }

    if (line_count == 0) {
        mvwprintw(func_win, 2, 2, "No URI found");
    } else {
        mvwprintw(func_win, 3, 2, "Select the URI to edit:");
        for (int i = 0; i < line_count; i++) {
            mvwprintw(func_win, i + 4, 2, "%d. %s", i + 1, lines[i]);
        }

    }
    return 0;
}


