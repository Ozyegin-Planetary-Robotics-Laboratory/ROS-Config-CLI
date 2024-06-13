#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <form.h>
#include <sys/types.h>
#include <pwd.h>
#include <vector>
#include <arpa/inet.h>

#include <ncurses.h>
#include <menu.h>
#include <cstdlib>

void view_message_at_info(WINDOW *info_win, const char *message,int x, int y) {
    wclear(info_win);
    if(x != 0 && y != 0)
        mvwprintw(info_win, y, x, message);
    else
        mvwprintw(info_win, 1, 1, message);
    wrefresh(info_win);
}

void draw_border(WINDOW *win){
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    refresh();  
}

static char* trim_whitespaces(char *str)
{
	char *end;

	// trim leading space
	while(isspace(*str))
		str++;

	if(*str == 0) // all spaces?
		return str;

	// trim trailing space
	end = str + strnlen(str, 128) - 1;

	while(end > str && isspace(*end))
		end--;

	// write new null terminator
	*(end+1) = '\0';

	return str;
}

static void driver(int ch,FORM *form,FIELD *fields[7],WINDOW * win_form)
{

	switch (ch) {
		case 10:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_PREV_FIELD);
            pos_form_cursor(form);
            break;

		case KEY_DOWN:
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_END_LINE);
			break;

		case KEY_UP:
			form_driver(form, REQ_PREV_FIELD);
			form_driver(form, REQ_END_LINE);
			break;

		case KEY_LEFT:
			form_driver(form, REQ_PREV_CHAR);
			break;

		case KEY_RIGHT:
			form_driver(form, REQ_NEXT_CHAR);
			break;

		// Delete the char before cursor
		case KEY_BACKSPACE:
		case 127:
			form_driver(form, REQ_DEL_PREV);
			break;

		// Delete the char under the cursor
		case KEY_DC:
			form_driver(form, REQ_DEL_CHAR);
			break;

		default:
			form_driver(form, ch);
			break;
	}

	wrefresh(win_form);
}

bool check_ip_validity(const char *ip) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return result != 0;
}


#endif
