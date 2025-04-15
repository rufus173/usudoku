#include "ncurses_interface.h"
#include <string.h>
#include "sudoku.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <curses.h>

#define SPLASH_DIALOGUE "q: quit, enter: confirm\narrows: move\nnumbers: enter number"

#define SUDOKU_WIDTH 37
#define SUDOKU_HEIGHT 19
#define DIALOGUE_HEIGHT 5

#define MIN_WIDTH SUDOKU_WIDTH
#define MIN_HEIGHT (SUDOKU_HEIGHT+DIALOGUE_HEIGHT)

/*
+-----------+ the top half if the actual board,
|           | and the bottom is dialogue for errors
| sudoku    | or hints.
| board     |
|           | this whole structure is centred
|           | in the middle of the terminal
+-----------+
| dialogue  |
| window    |
+-----------+
*/

int ncurses_sudoku(sudoku_t *sudoku){
	//====== initialisation of ncurses ======
	setlocale(LC_ALL,"");
	initscr();
	//====== check if it can fit in the terminal ======
	if (LINES < MIN_HEIGHT || COLS < MIN_WIDTH){
		printw("Terminal to small:\nlines %d - requires %d\ncolumns %d - requires %d\n",LINES,MIN_HEIGHT,COLS,MIN_WIDTH);
		getch();
		endwin();
		return EXIT_FAILURE;
	}
	//====== create windows ======
	WINDOW *sudoku_window = newwin(SUDOKU_HEIGHT,SUDOKU_WIDTH,(LINES/2)-(MIN_HEIGHT/2),(COLS/2)-(MIN_WIDTH/2));
	WINDOW *dialogue_window = newwin(DIALOGUE_HEIGHT,SUDOKU_WIDTH,(LINES/2)-(MIN_HEIGHT/2)+SUDOKU_HEIGHT,(COLS/2)-(MIN_WIDTH/2));
	box(dialogue_window,0,0);
	refresh();
	wrefresh(sudoku_window);
	wrefresh(dialogue_window);
	update_sudoku_window(sudoku_window,sudoku);

	//====== show help text ======
	update_dialogue_window(dialogue_window,SPLASH_DIALOGUE);

	getch();

	//====== stop ncurses ======
	endwin();
	delwin(sudoku_window);
	delwin(dialogue_window);
	return EXIT_SUCCESS;
}

void update_sudoku_window(WINDOW *sudoku_window,sudoku_t *sudoku){
	char *sudoku_as_string = sudoku__str__(sudoku);
	//====== split and print each line individualy ======
	char *line;
	char *next_line = sudoku_as_string;
	for (int i = 0;next_line != NULL;){
		line = next_line;
		next_line = strchr(line,'\n');
		if (next_line != NULL){
			next_line[0] = '\0';
			next_line++;
		}
		mvwprintw(sudoku_window,i,0,"%s",line);
		i++;
	}
	//====== highlight user input squares ======

	wrefresh(sudoku_window);
}
void update_dialogue_window(WINDOW *dialogue_window,char *text){
	
	//this function assumes the window has a box drawn around it

	//====== split and print each line individualy ======
	char *text_copy = strdup(text);
	char *line;
	char *next_line = text_copy;
	for (int i = 1; next_line != NULL;){
		line = next_line;
		next_line = strchr(line,'\n');
		if (next_line != NULL){
			next_line[0] = '\0';
			next_line++;
		}
		//====== wrap text ======
		int window_width;
		int window_height;
		getmaxyx(dialogue_window,window_height,window_width);
		window_width-=2;
		window_height-=1;
		int x = 1;
		for (int line_index = 0; line[line_index] != '\0'; line_index++){
			//dont print outside of the window
			if (i >= window_height) break;
			mvwprintw(dialogue_window,i,x,"%c",line[line_index]);
			x++;
			if (x > window_width){
				x = 1;
				i++;
			}
		}
		i++;
	}
	free(text_copy);
	wrefresh(dialogue_window);
}
